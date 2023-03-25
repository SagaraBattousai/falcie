package com.calotechnologies.faldroid.dataset.cifar10

import android.content.res.AssetManager
import android.util.Log
import com.calotechnologies.faldroid.dataset.Dataset
import com.calotechnologies.faldroid.model.Model
import com.calotechnologies.faldroid.utils.loadImage
import java.io.File
import java.io.FileInputStream
import java.nio.FloatBuffer
import java.nio.LongBuffer

class Cifar10Dataset : Dataset<FloatBuffer, LongBuffer>() {

    private val trainingDatasetFilenames = arrayOf(
        "data_batch_1.rgb", "data_batch_2.rgb",
        "data_batch_3.rgb", "data_batch_4.rgb", "data_batch_5.rgb"
    )

    private val testDatasetFilename = "test_batch.rgb"

    companion object {
        private const val TAG = "Cifar10DatasetTAG:"
        const val BATCH_SIZE: Int = 10000
        const val TRAINING_DATA_COUNT: Int = 50000
        const val TEST_DATA_COUNT: Int = 10000
        const val TOTAL_DATA_COUNT: Int = 60000
        private const val DATA_SIZE: Int = (32 * 32 * 3) + 1
        private const val INPUT_SIZE: Int = (32 * 32 * 3)
        private const val LABEL_SIZE: Int = 1
        private const val BATCH_FILE_SIZE: Int = DATA_SIZE * BATCH_SIZE
        private const val TRAINING_FILE_COUNT: Int = 5
        private const val TEST_FILE_COUNT: Int = 1

    }

    //Pre datapoint must be between 1 and 60,000 (i.e. 1 and number of training+test data
    fun getDatapoint(assetManager: AssetManager, datapoint: Int): Datapoint {
        var dataIndex = datapoint.coerceIn(1, TOTAL_DATA_COUNT) - 1
        val filename = if (dataIndex < TRAINING_DATA_COUNT)
            trainingDatasetFilenames[dataIndex / BATCH_SIZE]
        else
            testDatasetFilename

        dataIndex %= BATCH_SIZE

        val rawData = ByteArray(DATA_SIZE)
        //var inputStream = FileInputStream(assetManager.openFd(filename).fileDescriptor)
        var inputStream = assetManager.open(filename) // Apparently the file is being compressed
        inputStream.skip((dataIndex * DATA_SIZE).toLong())

        val readBytes = inputStream.read(rawData, 0, DATA_SIZE)
        Log.d(
            TAG,
            "readBytes: $readBytes MUST equal $DATA_SIZE! ${readBytes == DATA_SIZE}"
        )
        inputStream.close()

        val label = LongBuffer.allocate(LABEL_SIZE)
        label.put(rawData[0].toLong())

        val pixels = FloatBuffer.allocate(INPUT_SIZE)
        for (i in 1..INPUT_SIZE) {
            pixels.put(rawData[i].toFloat())
        }

        return Datapoint(pixels, label)
    }

    //Pre BatchSize must be a multiple of 50,000 AKA Training Data Size
    fun getTrainingData(
        assetManager: AssetManager,
        batchSize: Int = BATCH_SIZE,
        normalize: Boolean = false
    ): DataBatch {

        /* Load Raw Data into memory */

        //Is this a ridiculous way to do this? I guess its inefficient but is it bad?
        val rawTrainingData = ByteArray(BATCH_FILE_SIZE * TRAINING_FILE_COUNT)
        var currRead = 0
        var currInputStream: FileInputStream

        for (fname in trainingDatasetFilenames) {
            currInputStream = FileInputStream(assetManager.openFd(fname).fileDescriptor)
            val readBytes = currInputStream.read(rawTrainingData, currRead, BATCH_FILE_SIZE)
            Log.d(
                TAG,
                "readBytes: $readBytes MUST equal $BATCH_FILE_SIZE! ${readBytes == BATCH_FILE_SIZE}"
            )
            currRead += readBytes
            currInputStream.close()
        }

        /*
        val numberOfBatches =
            if ((TRAINING_DATA_COUNT % batchSize) == 0)
                TRAINING_DATA_COUNT / batchSize
            else (TRAINING_DATA_COUNT / batchSize) + 1
         */

        val numberOfBatches: Int = TRAINING_DATA_COUNT / batchSize

        val trainInputBatches: MutableList<FloatBuffer> = ArrayList(numberOfBatches)
        val trainLabelBatches: MutableList<LongBuffer> = ArrayList(numberOfBatches);

        var rawDataOffset: Int = 0

        for (i in 0 until numberOfBatches) {
            // Prepare training batches.
            val trainImages: FloatBuffer = FloatBuffer.allocate(batchSize * INPUT_SIZE)
            val trainLabels: LongBuffer = LongBuffer.allocate(batchSize)

            var it: Int = 0
            while (it < batchSize) {
                trainLabels.put(rawTrainingData[rawDataOffset].toLong())
                ++rawDataOffset
                for (j in rawDataOffset until INPUT_SIZE + rawDataOffset) {
                    trainImages.put(rawTrainingData[j].toFloat())
                }
                rawDataOffset += INPUT_SIZE
                ++it
            }
            trainImages.rewind()
            trainLabels.rewind()

            trainInputBatches.add(trainImages)
            trainLabelBatches.add(trainLabels)

        }
        return DataBatch(trainInputBatches, trainLabelBatches)
    }
}