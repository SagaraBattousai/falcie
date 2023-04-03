package com.calotechnologies.faldroid.model

import android.content.res.AssetManager
import android.util.Log
import com.calotechnologies.faldroid.blockchain.Blockchain
import com.calotechnologies.faldroid.dataset.DataBatch
import com.calotechnologies.faldroid.dataset.iterator.DatasetIterator
import com.calotechnologies.faldroid.utils.*
import org.tensorflow.lite.Interpreter
import java.io.File
import java.nio.FloatBuffer
import java.nio.LongBuffer
import java.nio.MappedByteBuffer

//TODO: Move back from Data Buffer
class Model(modelData: MappedByteBuffer) {

    private val model = Interpreter(modelData)

    //TODO: Increment in training! //Or not needed?????
    //private var examplesSeen: Long = 0L //Since last getWeights //getWeights resets to 0

    constructor(assetManager: AssetManager, modelFilename: String) :
            this(loadModelFile(assetManager, modelFilename))

    companion object {

        private const val TAG = "ModelClassTag"

        private const val TRAIN_SIGNATURE = "atrain" //beautiful hacky fix TODO: Inform tensorflow
        private const val TEST_SIGNATURE = "test"
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

        private const val TEST_LOSS_OUTPUT_PARAMETER_NAME = "loss"
        private const val TEST_ACCURACY_OUTPUT_PARAMETER_NAME = "accuracy"

        private const val NUM_CLASSES = 10
        private const val NUM_LAYERS = 5
        private const val NUM_WEIGHT_LAYERS = NUM_LAYERS * 2 //since kernel and bias are separate
        private const val IMG_WIDTH: Int = 32
        private const val IMG_HEIGHT: Int = 32
        private const val IMG_CHANNELS: Int = 3

        private const val DEFAULT_NUM_EPOCHS = 100
        private const val DEFAULT_BATCH_SIZE = 100

        private const val LAYER_1_KERNEL_PARAMETERS: Int = 3 * 3 * 3 * 32
        private const val LAYER_1_BIAS_PARAMETERS: Int = 32
        private const val LAYER_2_KERNEL_PARAMETERS: Int = 3 * 3 * 32 * 64
        private const val LAYER_2_BIAS_PARAMETERS: Int = 64
        private const val LAYER_3_KERNEL_PARAMETERS: Int = 3 * 3 * 64 * 64
        private const val LAYER_3_BIAS_PARAMETERS: Int = 64
        private const val LAYER_4_KERNEL_PARAMETERS: Int = 1024 * 64
        private const val LAYER_4_BIAS_PARAMETERS: Int = 64
        private const val LAYER_5_KERNEL_PARAMETERS: Int = 64 * 10
        private const val LAYER_5_BIAS_PARAMETERS: Int = 10

        const val MODEL_PARAMETERS = LAYER_1_KERNEL_PARAMETERS +
                LAYER_1_BIAS_PARAMETERS +
                LAYER_2_KERNEL_PARAMETERS +
                LAYER_2_BIAS_PARAMETERS +
                LAYER_3_KERNEL_PARAMETERS +
                LAYER_3_BIAS_PARAMETERS +
                LAYER_4_KERNEL_PARAMETERS +
                LAYER_4_BIAS_PARAMETERS +
                LAYER_5_KERNEL_PARAMETERS +
                LAYER_5_BIAS_PARAMETERS

        fun newWeightBuffer(): Array<FloatBuffer> {
            return arrayOf(
                directAllocateNativeFloatBuffer(LAYER_1_KERNEL_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_1_BIAS_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_2_KERNEL_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_2_BIAS_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_3_KERNEL_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_3_BIAS_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_4_KERNEL_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_4_BIAS_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_5_KERNEL_PARAMETERS),
                directAllocateNativeFloatBuffer(LAYER_5_BIAS_PARAMETERS)
            )
        }

        private fun setWeightReference(
            weights: Array<FloatBuffer>,
            map: MutableMap<String, Any>
        ) {
            for (i in weights.indices step 2) {
                map["layer_${(i / 2) + 1}_kernel"] = weights[i]
                //since we know float buffer must have even size we don't
                // need to check for out of range
                map["layer_${(i / 2) + 1}_bias"] = weights[i + 1]
            }
        }

        //Why did i make this??? I don't think its needed, Where the heck has today gone :O,
        // I Was on such a roll what happened today???
        private fun getWeightsFromReference(map: MutableMap<String, Any>): Array<FloatBuffer> {
            return Array<FloatBuffer>(NUM_WEIGHT_LAYERS) {
                if (it % 2 == 0)
                    map["layer_${(it / 2) + 1}_kernel"] as FloatBuffer
                else
                    map["layer_${(it / 2) + 1}_bias"] as FloatBuffer
            }
        }


        //Technically a static/class method as A) applies to many models and B) saveFedWeights
        //is the exact same function regardless of model instance (as long as they are all of the
        //same type/Architecture
        //Also NOTE: Naughtily allows access to private members much like friend func
        //Note: you probably want to rewind all buffers before calling this func
        fun setFederatedWeights(
            models: Array<Model>, //Must have size > 0
            federatedWeights: Array<FloatBuffer>,
            fileRoot: File
        ) {
            //could make out? //want to be immutable later
            val federatedSaveInputs: MutableMap<String, Any> = HashMap()
            setWeightReference(federatedWeights, federatedSaveInputs)

            val federatedWeightsFile = File(fileRoot, FEDERATED_WEIGHTS_SAVE_FILENAME)

            federatedSaveInputs[SAVE_AND_RESTORE_PARAMETER_NAME] = federatedWeightsFile.absolutePath

            //Acceptable to be unreferenced hashmap? It returns despite our lack of interest
            models[0].model.runSignature(
                federatedSaveInputs, HashMap(), SAVE_FEDERATED_WEIGHTS_SIGNATURE
            )

            val federatedRestoreInputs: MutableMap<String, Any> = HashMap()
            federatedRestoreInputs["checkpoint_path"] = federatedWeightsFile.absolutePath

            for (m in models) {
                m.restore(federatedWeightsFile)
            }
        }

        fun federatedTrain(filesDir: File, models: Array<Model>, datasetIterator: DatasetIterator<FloatBuffer, LongBuffer>, blockchain: Blockchain, epochBlockchainSubmission: Int, numEpochs: Int = 50) {
            val numModels = models.size

            val examplesSeen = LongArray(numModels)
            val batchSplits = IntArray(numModels) {(it + 1) * datasetIterator.length / numModels}

            var currModel: Model

            val loss: FloatBuffer = directAllocateNativeFloatBuffer(1)
            var modelIndex: Int
            var datapointIndex: Int

            for(epoch in 0 until numEpochs) {
                modelIndex = 0
                datapointIndex = 0
                datasetIterator.reset()

                while (datasetIterator.hasNext()) {
                    val dataBatch = datasetIterator.next()

                    //Not guaranteed if uneven batch splits
                    if (datapointIndex >= batchSplits[modelIndex])
                        ++modelIndex

                    currModel = models[modelIndex]
                    examplesSeen[modelIndex] += dataBatch.length.toLong()

                    currModel.train(dataBatch, loss)
                    loss.rewind()
                    ++datapointIndex
                }

                if ((epoch + 1) % epochBlockchainSubmission == 0)
                {
                    Log.v(TAG, "Generating Federated Weights with blockchain")
                    Log.v(TAG, "Loss at epoch: ${epoch + 1} is ${loss[0]}")

                    val allWeights = Array<Array<FloatBuffer>>(numModels) {
                        models[it].getWeights()
                    }
                    val federatedWeights = blockchain.federate(allWeights, examplesSeen)
                    setFederatedWeights(models, federatedWeights, filesDir)
                }

                Log.d(TAG, "Epoch complete")

                if ((epoch + 1) % 10 == 0) {
                    Log.v(TAG, "Finished ${epoch + 1} epochs, current loss: ${loss[0]}")
                }
            }
        }
    }

    fun getWeights(): Array<FloatBuffer> {//FederatedWeights<FloatBuffer> {
        val inputs: Map<String, Any> = mapOf("unused" to "")

        val outputs: MutableMap<String, Any> = HashMap()
        val weights: Array<FloatBuffer> = newWeightBuffer()

        setWeightReference(weights, outputs)

        model.runSignature(inputs, outputs, GET_WEIGHTS_SIGNATURE)

        //val fedWeights = FederatedWeights(weights, examplesSeen)
        //examplesSeen = 0

        return weights //fedWeights
    }


    fun save(fileRoot: File, checkpointPath: String) {
        save(File(fileRoot, checkpointPath))
    }

    fun save(checkpointFile: File) {
        val inputs: MutableMap<String, Any> = HashMap()
        inputs[SAVE_AND_RESTORE_PARAMETER_NAME] = checkpointFile.absolutePath
        //TODO: Try mapOf
        model.runSignature(inputs, HashMap(), SAVE_SIGNATURE)
    }

    fun restore(fileRoot: File, checkpointPath: String) {
        restore(File(fileRoot, checkpointPath))
    }

    fun restore(checkpointFile: File) {
        val inputs: MutableMap<String, Any> = HashMap()
        inputs[SAVE_AND_RESTORE_PARAMETER_NAME] = checkpointFile.absolutePath
        //TODO: Try mapOf
        //val outputs: Map<String, Any> = HashMap()
        model.runSignature(inputs, HashMap(), RESTORE_SIGNATURE)
    }

    //Single batch training
    fun train(dataBatch: DataBatch<FloatBuffer, LongBuffer>, loss: FloatBuffer) {

        //Log.v(TAG, "Single Batch Training Starting")

        //while (datasetIterator.hasNext()) {
        val inputs: Map<String, Any> = mapOf(
            MODEL_INPUT_PARAMETER_NAME to dataBatch.x,
            MODEL_LABEL_PARAMETER_NAME to dataBatch.y
        )

        val outputs: Map<String, Any> = mapOf(TRAIN_LOSS_OUTPUT_PARAMETER_NAME to loss)

        //X is 1 for some reason TODO: use get index
        model.resizeInput(1, intArrayOf(dataBatch.batchSize, 32, 32, 3), true)
        model.resizeInput(0, intArrayOf(dataBatch.batchSize), true)
        model.resetVariableTensors()

        model.runSignature(inputs, outputs, TRAIN_SIGNATURE)

        //loss.rewind() //Don't think i need!

        //Log.v(TAG, "Single Batch Training Complete")
    }

    //Unused Me thinks
    fun train(
        assetManager: AssetManager,
        trainingImageFilenames: Array<String>,
        trainingLabels: LongArray,
        numberOfEpochs: Int = DEFAULT_NUM_EPOCHS,
        batchSize: Int = DEFAULT_BATCH_SIZE,
        normalize: Boolean = false


    ): FloatArray {
        val trainingDataCount: Int = trainingImageFilenames.size

        val numberOfBatches = trainingDataCount / batchSize

        val trainInputBatches: Array<FloatBuffer> = Array(numberOfBatches) {
            directAllocateNativeFloatBuffer(batchSize * IMG_WIDTH * IMG_HEIGHT * IMG_CHANNELS)
        }
        val trainLabelBatches: Array<LongBuffer> = Array(numberOfBatches) {
            directAllocateNativeLongBuffer(batchSize)
        }

        for (i in 0 until numberOfBatches) {
            // Prepare training batches.
            //val trainImages: FloatBuffer = FloatBuffer.allocate()

            val offset = i * batchSize

            //val trainLabels: LongBuffer = LongBuffer.allocate(batchSize).put(trainingLabels, offset, batchSize)
            trainLabelBatches[i].put(trainingLabels, offset, batchSize)

            for (j in offset until offset + batchSize) {
                trainInputBatches[i].put(
                    loadImage(
                        assetManager, trainingImageFilenames[j],
                        IMG_WIDTH, IMG_HEIGHT, IMG_CHANNELS, normalize
                    )
                )
            }
            trainInputBatches[i].rewind()
            trainLabelBatches[i].rewind()
        }
        return train(trainInputBatches, trainLabelBatches, numberOfBatches, numberOfEpochs)
    }

    fun train(
        trainInputBatches: Array<FloatBuffer>,
        trainLabelBatches: Array<LongBuffer>,
        numberOfBatches: Int,
        numberOfEpochs: Int = DEFAULT_NUM_EPOCHS,
    ): FloatArray {

        // Run training for a few steps.
        val losses = FloatArray(numberOfEpochs)
        val loss: FloatBuffer = directAllocateNativeFloatBuffer(1)

        for (epoch in 0 until numberOfEpochs) {
            for (batchIndex in 0 until numberOfBatches) {
                val dataBatch = DataBatch(
                    trainInputBatches[batchIndex],
                    trainLabelBatches[batchIndex],
                    numberOfBatches
                )

                train(dataBatch, loss)
                loss.rewind()
            }
            losses[epoch] = loss[0]

            if ((epoch + 1) % 10 == 0) {
                Log.v(TAG, "Finished ${epoch + 1} epochs, current loss: ${losses[epoch]}")
            }
        }
        return losses
    }

    fun train(
        datasetIterator: DatasetIterator<FloatBuffer, LongBuffer>,
        numberOfEpochs: Int = DEFAULT_NUM_EPOCHS,
    ): FloatArray {

        // Run training for a few steps.
        val losses = FloatArray(numberOfEpochs)
        val loss: FloatBuffer = directAllocateNativeFloatBuffer(1)


        Log.v(TAG, "Training Starting")
        for (epoch in 0 until numberOfEpochs) {
            datasetIterator.reset()
            while (datasetIterator.hasNext()) {
                train(datasetIterator.next(), loss)
                loss.rewind()
            }

            //record final loss
            losses[epoch] = loss[0]
            Log.d(TAG, "Epoch complete")

            if ((epoch + 1) % 10 == 0) {
                Log.v(TAG, "Finished ${epoch + 1} epochs, current loss: ${losses[epoch]}")
            }

        }
        return losses
    }

    //////////////////////////////////////////
    //Single batch testing
    fun test(
        dataBatch: DataBatch<FloatBuffer, LongBuffer>,
        loss: FloatBuffer,
        accuracy: FloatBuffer
    ) {

        val inputs: Map<String, Any> = mapOf(
            MODEL_INPUT_PARAMETER_NAME to dataBatch.x,
            MODEL_LABEL_PARAMETER_NAME to dataBatch.y
        )

        val outputs: Map<String, Any> = mapOf(
            TEST_LOSS_OUTPUT_PARAMETER_NAME to loss,
            TEST_ACCURACY_OUTPUT_PARAMETER_NAME to accuracy
        )

        //TODO: Will this work with tensorflow's .....isssue So It don't work!!!!!!!! Naughty tf!!
        //X is 1 for some reason TODO: use get index
        model.resizeInput(1, intArrayOf(dataBatch.batchSize, 32, 32, 3), true)
        model.resizeInput(0, intArrayOf(dataBatch.batchSize), true)
        model.resetVariableTensors()

        model.runSignature(inputs, outputs, TEST_SIGNATURE)

        //loss.rewind() //Don't think i need!

        //Log.v(TAG, "Single Batch Training Complete")
    }
    //////////////////////////////////////////


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

        model.runSignature(inputs, outputs, INFER_SIGNATURE)

        return outputs
    }


    fun trainInputNames(): Array<String> = model.getSignatureInputs(TRAIN_SIGNATURE)

    fun getSignatureInputs(signatureKey: String): Array<String> =
        model.getSignatureInputs(signatureKey)

}