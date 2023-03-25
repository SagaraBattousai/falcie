package com.calotechnologies.faldroid.model

import android.content.res.AssetManager
import android.util.Log
import com.calotechnologies.faldroid.utils.loadImage
import com.calotechnologies.faldroid.utils.loadModelFile
import org.tensorflow.lite.Interpreter
import java.io.File
import java.nio.FloatBuffer
import java.nio.LongBuffer
import java.nio.MappedByteBuffer

class Model(modelData: MappedByteBuffer) {

    private val model = Interpreter(modelData)

    constructor(assetManager: AssetManager, modelFilename: String) :
            this(loadModelFile(assetManager, modelFilename)) {
    }

    companion object {

        private const val TAG = "ModelClassTag"

        private const val TRAIN_SIGNATURE = "train"
        private const val INFER_SIGNATURE = "infer"
        private const val SAVE_SIGNATURE = "save"
        private const val RESTORE_SIGNATURE = "restore"
        private const val GET_WEIGHTS_SIGNATURE = "getWeights" //may change value name
        private const val SAVE_FEDERATED_WEIGHTS_SIGNATURE = "saveFedWeights"
        private const val SAVE_AND_RESTORE_PARAMETER_NAME = "checkpoint_path"
        private const val MODEL_INPUT_PARAMETER_NAME = "x"
        private const val MODEL_LABEL_PARAMETER_NAME = "y"
        private const val FEDERATED_WEIGHTS_SAVE_FILENAME = "federated_weights_save_file.ckpt"

        private const val INFER_PROBABILITIES_OUTPUT_PARAMETER_NAME = "output"
        private const val INFER_LOGITS_OUTPUT_PARAMETER_NAME = "logits"

        private const val TRAIN_LOSS_OUTPUT_PARAMETER_NAME = "loss"

        private const val NUM_CLASSES = 10
        private const val NUM_WEIGHT_LAYERS = 10 //come up with better name
        private const val IMG_WIDTH: Int = 32;
        private const val IMG_HEIGHT: Int = 32;
        private const val IMG_CHANNELS: Int = 3;

        private const val DEFAULT_NUM_EPOCHS = 100
        private const val DEFAULT_BATCH_SIZE = 100

        private fun newWeightBuffer(): List<FloatBuffer> {
            return listOf(
                FloatBuffer.allocate(3 * 3 * 3 * 32),
                FloatBuffer.allocate(32),
                FloatBuffer.allocate(3 * 3 * 32 * 64),
                FloatBuffer.allocate(64),
                FloatBuffer.allocate(3 * 3 * 64 * 64),
                FloatBuffer.allocate(64),
                FloatBuffer.allocate(1024 * 64),
                FloatBuffer.allocate(64),
                FloatBuffer.allocate(64 * 10),
                FloatBuffer.allocate(10)
            )
        }

        private fun setWeightReference(
            weights: List<FloatBuffer>,
            map: MutableMap<String, Any>
        ): Unit {
            for (i in weights.indices step 2) {
                map["layer_${(i / 2) + 1}_kernel"] = weights[i]
                //since we know float buffer must have even size we don't
                // need to check for out of range
                map["layer_${(i / 2) + 1}_bias"] = weights[i + 1]
            }
        }

        //Technically a static/class method as A) applies to many models and B) saveFedWeights
        //is the exact same function regardless of model instance (as long as they are all of the
        //same type/Architecture
        //Also NOTE: Naughtily allows access to private members much like friend func
        //Note: you probably want to rewind all buffers before calling this func
        fun setFederatedWeights(
            models: List<Model>, //Must have size > 0
            federatedWeights: List<FloatBuffer>,
            fileRoot: File
        ): Unit {
            //could make out? //want to be immutable later
            val federatedSaveInputs: MutableMap<String, Any> = HashMap()
            setWeightReference(federatedWeights, federatedSaveInputs)

            //val outputs: MutableMap<String, Any> = HashMap()

            val federatedWeightsFile = File(fileRoot, FEDERATED_WEIGHTS_SAVE_FILENAME)

            federatedSaveInputs[SAVE_AND_RESTORE_PARAMETER_NAME] = federatedWeightsFile.absolutePath

            //Acceptable to be unreferenced hashmap? It returns despite our lack of interest
            models[0].model.runSignature(
                federatedSaveInputs, HashMap(), SAVE_FEDERATED_WEIGHTS_SIGNATURE
            )

            val federatedRestoreInputs: MutableMap<String, Any> = HashMap()
            federatedRestoreInputs["checkpoint_path"] = federatedWeightsFile.absolutePath

            //val outputs3: MutableMap<String, Any> = HashMap()

            for (m in models) {
                m.restore(federatedWeightsFile)
            }
        }

    }

    fun getWeights(): List<FloatBuffer> {
        val inputs: MutableMap<String, Any> = HashMap()
        inputs["unused"] = ""

        val outputs: MutableMap<String, Any> = HashMap()
        val weights: List<FloatBuffer> = newWeightBuffer()

        setWeightReference(weights, outputs)

        model.runSignature(inputs, outputs, GET_WEIGHTS_SIGNATURE);

        return weights
    }

    fun restore(fileRoot: File, checkpointPath: String): Unit {
        restore(File(fileRoot, checkpointPath))
    }

    fun restore(checkpointFile: File): Unit {
        val inputs: MutableMap<String, Any> = HashMap()
        inputs[SAVE_AND_RESTORE_PARAMETER_NAME] = checkpointFile.absolutePath
        //TODO: Try mapOf
        //val outputs: Map<String, Any> = HashMap()
        model.runSignature(inputs, HashMap(), RESTORE_SIGNATURE)
    }

    fun train(
        assetManager: AssetManager,
        trainingImageFilenames: Array<String>,
        trainingLabels: LongArray,
        numberOfEpochs: Int = DEFAULT_NUM_EPOCHS,
        batchSize: Int = DEFAULT_BATCH_SIZE,
        normalize: Boolean = false
    ) : FloatArray {
        val trainingDataCount: Int = trainingImageFilenames.size

        val numberOfBatches = trainingDataCount / batchSize

        val trainInputBatches: MutableList<FloatBuffer> = ArrayList(numberOfBatches)
        val trainLabelBatches: MutableList<LongBuffer> = ArrayList(numberOfBatches);

        for (i in 0 until numberOfBatches) {
            // Prepare training batches.
            val trainImages: FloatBuffer =
                FloatBuffer.allocate(batchSize * IMG_WIDTH * IMG_HEIGHT * IMG_CHANNELS)

            val offset = i * batchSize

            val trainLabels: LongBuffer =
                LongBuffer.allocate(batchSize).put(trainingLabels, offset, batchSize)

            for (j in offset until offset + batchSize) {
                trainImages.put(
                    loadImage(
                        assetManager, trainingImageFilenames[j],
                        IMG_WIDTH, IMG_HEIGHT, IMG_CHANNELS, normalize
                    )
                )
            }
            trainImages.rewind()
            trainLabels.rewind()

            trainInputBatches.add(trainImages)
            trainLabelBatches.add(trainLabels)

        }
        return train(trainInputBatches, trainLabelBatches, numberOfBatches, numberOfEpochs)
    }

    fun train(
        trainInputBatches: MutableList<FloatBuffer>,
        trainLabelBatches: MutableList<LongBuffer>,
        numberOfBatches: Int,
        numberOfEpochs: Int = DEFAULT_NUM_EPOCHS,
    ) : FloatArray {

        // Run training for a few steps.
        val losses = FloatArray(numberOfEpochs)

        for (epoch in 0 until numberOfEpochs) {
            for (batchIndex in 0 until numberOfBatches) {
                val inputs: MutableMap<String, Any> = HashMap()
                inputs[MODEL_INPUT_PARAMETER_NAME] = trainInputBatches[batchIndex]
                inputs[MODEL_LABEL_PARAMETER_NAME] = trainLabelBatches[batchIndex]

                val outputs: MutableMap<String, Any> = HashMap()

                val loss = FloatBuffer.allocate(1)
                outputs[TRAIN_LOSS_OUTPUT_PARAMETER_NAME] = loss

                model.runSignature(inputs, outputs, TRAIN_SIGNATURE)

                //record final loss
                if (batchIndex == numberOfBatches - 1)
                    losses[epoch] = loss[0]
            }
            if ((epoch + 1) % 10 == 0) {
                Log.v(TAG, "Finished ${epoch + 1} epochs, current loss: ${losses[epoch]}")
            }

        }
        return losses
    }

    fun infer(
        assetManager: AssetManager, imagePath: String,
        normalize: Boolean = false
    ): MutableMap<String, Any> {
        //May fail if image doesn't have the the correct size
        return infer(
            loadImage(
                assetManager, imagePath,
                IMG_WIDTH, IMG_HEIGHT, IMG_CHANNELS,
                normalize
            )
        )

    }

    //May change map value type
    fun infer(inputData: FloatBuffer): MutableMap<String, Any> {
        val inputs: MutableMap<String, Any> = HashMap()
        inputs[MODEL_INPUT_PARAMETER_NAME] = inputData

        val outputs: MutableMap<String, Any> = HashMap()

        val probabilities: FloatBuffer = FloatBuffer.allocate(NUM_CLASSES)
        val logits: FloatBuffer = FloatBuffer.allocate(NUM_CLASSES)

        outputs[INFER_PROBABILITIES_OUTPUT_PARAMETER_NAME] = probabilities
        outputs[INFER_LOGITS_OUTPUT_PARAMETER_NAME] = logits

        model.runSignature(inputs, outputs, INFER_SIGNATURE);

        return outputs
    }

    fun save(fileRoot: File, checkpointPath: String): Unit {
        save(File(fileRoot, checkpointPath))
    }

    fun save(checkpointFile: File): Unit {
        val inputs: MutableMap<String, Any> = HashMap()
        inputs[SAVE_AND_RESTORE_PARAMETER_NAME] = checkpointFile.absolutePath
        //TODO: Try mapOf
        model.runSignature(inputs, HashMap(), RESTORE_SIGNATURE)
    }


}