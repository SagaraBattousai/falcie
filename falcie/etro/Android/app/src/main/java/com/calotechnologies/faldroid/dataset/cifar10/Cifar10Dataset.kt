package com.calotechnologies.faldroid.dataset.cifar10

import android.content.res.AssetManager
import android.util.Log
import com.calotechnologies.faldroid.dataset.*
import com.calotechnologies.faldroid.dataset.cifar10.iterator.Cifar10DataBatchIterator
import com.calotechnologies.faldroid.dataset.cifar10.iterator.Cifar10TestBatchIterator
import com.calotechnologies.faldroid.dataset.cifar10.iterator.Cifar10TrainingBatchIterator
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeLongBuffer
import com.calotechnologies.faldroid.utils.Math
import java.io.InputStream
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import java.nio.LongBuffer

class Cifar10Dataset(private val assetManager: AssetManager) : Dataset<FloatBuffer, LongBuffer> {
    companion object {
        private const val TAG = "Cifar10DatasetTAG:"

        internal val trainingDatasetFilenames = arrayOf(
            "data_batch_1.rgb", "data_batch_2.rgb",
            "data_batch_3.rgb", "data_batch_4.rgb", "data_batch_5.rgb"
        )
        internal const val testDatasetFilename = "test_batch.rgb"

        internal const val INPUT_SIZE: Int = (32 * 32 * 3)
        internal const val LABEL_SIZE: Int = 1
        internal const val DATA_SIZE: Int = INPUT_SIZE + LABEL_SIZE

        private const val TRAINING_FILE_COUNT: Int = 5
        private const val TEST_FILE_COUNT: Int = 1

        const val TRAINING_DATA_COUNT: Int = 50000
        const val TEST_DATA_COUNT: Int = 10000
        const val TOTAL_DATA_COUNT: Int = TRAINING_DATA_COUNT + TEST_DATA_COUNT //60000

        const val BATCH_SIZE: Int = 10000
        internal const val BATCH_FILE_SIZE: Int = DATA_SIZE * BATCH_SIZE
    }

    var batchSize: Int = BATCH_SIZE

    var dataPercent = 1.0f
        set(value) {
            field = value.coerceIn(0.0f, 1.0f)
        }

    var normalize: Boolean = false

    //Pre datapoint must be between 1 and 60,000 (i.e. 1 and number of training+test data
    fun getDatapoint(
        assetManager: AssetManager,
        datapoint: Int,
        normalize: Boolean = this.normalize
    ): Datapoint<FloatBuffer, LongBuffer> {

        var dataIndex = datapoint.coerceIn(1, TOTAL_DATA_COUNT) - 1
        val filename = if (dataIndex < TRAINING_DATA_COUNT)
            trainingDatasetFilenames[dataIndex / BATCH_SIZE]
        else
            testDatasetFilename

        dataIndex %= BATCH_SIZE

        val rawData = ByteArray(DATA_SIZE)
        //var inputStream = FileInputStream(assetManager.openFd(filename).fileDescriptor)
        val inputStream = assetManager.open(filename) // Apparently the file is being compressed
        inputStream.skip((dataIndex * DATA_SIZE).toLong())

        val readBytes = inputStream.read(rawData, 0, DATA_SIZE)
        Log.d(
            TAG,
            "readBytes: $readBytes MUST equal $DATA_SIZE! ${readBytes == DATA_SIZE}"
        )
        inputStream.close()

        //LongBuffer.allocate(LABEL_SIZE)
        val label: LongBuffer = directAllocateNativeLongBuffer(LABEL_SIZE)

        label.put(rawData[0].toLong())

        //FloatBuffer.allocate(INPUT_SIZE)
        val pixels: FloatBuffer = directAllocateNativeFloatBuffer(INPUT_SIZE)

        for (i in 1..INPUT_SIZE) {
            if (normalize)
                pixels.put(rawData[i].toFloat() / 255.0f)
            else
                pixels.put(rawData[i].toFloat())
        }

        pixels.rewind()
        label.rewind()

        return Datapoint(pixels, label)
    }

    override fun iterator(): TrainingTestPair<FloatBuffer, LongBuffer> =
        Pair(
            Cifar10TrainingBatchIterator(assetManager, dataPercent, batchSize, normalize),
            Cifar10TestBatchIterator(assetManager, dataPercent, batchSize, normalize)
        )
}