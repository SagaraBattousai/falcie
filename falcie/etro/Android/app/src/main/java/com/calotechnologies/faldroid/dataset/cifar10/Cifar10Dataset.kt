package com.calotechnologies.faldroid.dataset.cifar10

import android.content.res.AssetManager
import android.util.Log
import com.calotechnologies.faldroid.dataset.*
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeLongBuffer
import com.calotechnologies.faldroid.utils.Math
import java.io.InputStream
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import java.nio.LongBuffer

//File private variables (To allow iterator to be non inner class)
private val trainingDatasetFilenames = arrayOf(
    "data_batch_1.rgb", "data_batch_2.rgb",
    "data_batch_3.rgb", "data_batch_4.rgb", "data_batch_5.rgb"
)
private const val testDatasetFilename = "test_batch.rgb"

private const val INPUT_SIZE: Int = (32 * 32 * 3)
private const val LABEL_SIZE: Int = 1
private const val DATA_SIZE: Int = INPUT_SIZE + LABEL_SIZE

private const val TRAINING_FILE_COUNT: Int = 5
private const val TEST_FILE_COUNT: Int = 1


class Cifar10Dataset(private val assetManager: AssetManager) : Dataset<FloatBuffer, LongBuffer> {
    companion object {
        private const val TAG = "Cifar10DatasetTAG:"

        const val TRAINING_DATA_COUNT: Int = 50000
        const val TEST_DATA_COUNT: Int = 10000
        const val TOTAL_DATA_COUNT: Int = TRAINING_DATA_COUNT + TEST_DATA_COUNT //60000

        const val BATCH_SIZE: Int = 10000
        private const val BATCH_FILE_SIZE: Int = DATA_SIZE * BATCH_SIZE
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
    override fun iterator(): Cifar10DataBatchIterator =
        Cifar10DataBatchIterator(assetManager, dataPercent, batchSize, normalize)

    //TODO: Make cleaner interface messy af but today has been painfully unproductive
    //E.G. Could make batchSize lateinit etc but ... this is hard
    //Could make outer but meh
    class Cifar10DataBatchIterator(
        private val assetManager: AssetManager,
        dataPercent: Float = 1.0f,
        initialBatchSize: Int = BATCH_SIZE,
        val normalize: Boolean = false
    ) : DatasetIterator<FloatBuffer, LongBuffer> {

        var hasStarted = false
            private set

        var batchSize: Int = initialBatchSize
            private set

        //AKA number of images
        override val length: Int = (TRAINING_DATA_COUNT * dataPercent).toInt()
        //^^ Note this adds complexity of final batch as size will be smaller

        //VV Actually may not need, I really am tired and hungry!!
        // VV Will need changing in batch fun
        var numberOfBatches: Int = recalculateNumberOfBatches()
            private set

        var batchSplits: IntArray = recalculateBatchSplits()
            private set

        private var currentBatchIndex: Int = 0
        private var amountRead: Int = 0
        private var wholeFilesRead: Int = 0
        private var readBuffer = ByteArray(BATCH_FILE_SIZE) //AKA cache!
        private var bufferOffset: Int = 0

        //cant call get or set because of jvm interop
        private fun recalculateNumberOfBatches(): Int = Math.ceilDiv(length, batchSize)

        //cant call get or set because of jvm interop
        private fun recalculateBatchSplits(): IntArray {
            val splits = IntArray(numberOfBatches) { batchSize }

            val remainingSplit = length % batchSize
            if (remainingSplit != 0)
                splits[numberOfBatches - 1] = remainingSplit

            return splits
        }

        fun batch(batchSize: Int): Cifar10DataBatchIterator {
            if (!hasStarted) {
                this.batchSize = batchSize
                numberOfBatches = recalculateNumberOfBatches()
                batchSplits = recalculateBatchSplits()
            } else {
                Log.w(
                    TAG,
                    "Iterator has started, if you wish to change the batch size call reset" +
                            " first and then recall this function \"batch($batchSize)\""
                )
            }
            return this
        }

        override fun hasNext(): Boolean = amountRead < length

        override fun reset(): Cifar10DataBatchIterator {
            hasStarted = false
            amountRead = 0
            wholeFilesRead = 0
            bufferOffset = 0
            currentBatchIndex = 0

            return this
        }


        private fun readNextChunk() {
            val inputStream = assetManager.open(trainingDatasetFilenames[wholeFilesRead])
            inputStream.read(readBuffer, 0, BATCH_FILE_SIZE)
            inputStream.close()
        }

        override fun next(): DataBatch<FloatBuffer, LongBuffer> {
            //assert wholeFilesRead < 5 on entry on exit it may??

            if (!hasNext())
                throw NoSuchElementException("Iterator Has Finished")

            hasStarted = true

            val currentBatchSize = batchSplits[currentBatchIndex]

            val trainBatch = directAllocateNativeFloatBuffer(currentBatchSize * INPUT_SIZE)
            val labelBatch = directAllocateNativeLongBuffer(currentBatchSize * LABEL_SIZE)

            for (i in 0 until currentBatchSize) {

                if (bufferOffset == 0)
                    readNextChunk()

                labelBatch.put(readBuffer[bufferOffset].toLong())
                ++bufferOffset

                for (j in 0 until INPUT_SIZE) {
                    var pixelChannel: Float = readBuffer[j + bufferOffset].toFloat()
                    if (normalize)
                        pixelChannel /= 255.0f
                    trainBatch.put(pixelChannel)
                }
                bufferOffset += INPUT_SIZE

                ++amountRead

                //TODO: Check this is correct (although it should be it is late and im hungry)
                if (bufferOffset == BATCH_FILE_SIZE) {
                    bufferOffset = 0
                    ++wholeFilesRead
                    //readNextChunk()
                }
            }

            trainBatch.rewind()
            labelBatch.rewind()

            ++currentBatchIndex
            return DataBatch(trainBatch, labelBatch, currentBatchSize)
        }
    }

}