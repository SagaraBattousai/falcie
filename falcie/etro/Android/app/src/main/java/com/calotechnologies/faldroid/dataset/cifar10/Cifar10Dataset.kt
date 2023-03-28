package com.calotechnologies.faldroid.dataset.cifar10

import android.content.res.AssetManager
import android.util.Log
import android.util.NoSuchPropertyException
import com.calotechnologies.faldroid.dataset.*
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeLongBuffer
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
private const val TRAINING_DATA_COUNT: Int = 50000
private const val TEST_DATA_COUNT: Int = 10000
private const val TOTAL_DATA_COUNT: Int = TRAINING_DATA_COUNT + TEST_DATA_COUNT //60000

private const val INPUT_SIZE: Int = (32 * 32 * 3)
private const val LABEL_SIZE: Int = 1
private const val DATA_SIZE: Int = INPUT_SIZE + LABEL_SIZE

private const val TRAINING_FILE_COUNT: Int = 5
private const val TEST_FILE_COUNT: Int = 1


class Cifar10Dataset(private val assetManager: AssetManager) : Dataset<FloatBuffer, LongBuffer> {
    companion object {
        private const val TAG = "Cifar10DatasetTAG:"
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
        var inputStream = assetManager.open(filename) // Apparently the file is being compressed
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

    //Pre BatchSize must be a multiple of 50,000 AKA Training Data Size
    fun getTrainingData(
        assetManager: AssetManager,
        batchSize: Int = this.batchSize,
        normalize: Boolean = this.normalize
    ): BatchedDataset<FloatBuffer, LongBuffer> {

        /* Load Raw Data into memory */

        //Is this a ridiculous way to do this? I guess its inefficient but is it bad?
        val rawTrainingData = ByteArray(BATCH_FILE_SIZE)// * TRAINING_FILE_COUNT)
        var currRead = 0
        var currInputStream: InputStream

        //for (fname in trainingDatasetFilenames) {
        //TODO TMP
        val fname = trainingDatasetFilenames[0]
        //currInputStream = FileInputStream(assetManager.openFd(fname).fileDescriptor)
        currInputStream = assetManager.open(fname)
        val readBytes = currInputStream.read(rawTrainingData, currRead, BATCH_FILE_SIZE)
        currInputStream.close()
        Log.d(
            TAG,
            "readBytes: $readBytes MUST equal $BATCH_FILE_SIZE! ${readBytes == BATCH_FILE_SIZE}"
        )
        currRead += readBytes
        currInputStream.close()
        //}

        /*
        val numberOfBatches =
            if ((TRAINING_DATA_COUNT % batchSize) == 0)
                TRAINING_DATA_COUNT / batchSize
            else (TRAINING_DATA_COUNT / batchSize) + 1
         */

        //val numberOfBatches: Int = TRAINING_DATA_COUNT / batchSize
        //TODO: UNDO
        val numberOfBatches: Int = BATCH_SIZE / batchSize

        val trainInputBatches: MutableList<FloatBuffer> = ArrayList(numberOfBatches)
        val trainLabelBatches: MutableList<LongBuffer> = ArrayList(numberOfBatches);

        var rawDataOffset: Int = 0

        val nativeByteOrder = ByteOrder.nativeOrder()

        for (i in 0 until numberOfBatches) {
            // Prepare training batches.

            //FloatBuffer.allocate(batchSize * INPUT_SIZE)
            val trainImages: FloatBuffer = ByteBuffer
                .allocateDirect(batchSize * INPUT_SIZE * Float.SIZE_BYTES)
                .order(nativeByteOrder)
                .asFloatBuffer()

            //LongBuffer.allocate(batchSize)
            val trainLabels: LongBuffer = ByteBuffer
                .allocateDirect(batchSize * Long.SIZE_BYTES)
                .order(nativeByteOrder)
                .asLongBuffer()

            var it: Int = 0
            while (it < batchSize) {
                trainLabels.put(rawTrainingData[rawDataOffset].toLong())
                ++rawDataOffset
                for (j in rawDataOffset until INPUT_SIZE + rawDataOffset) {
                    if (normalize)
                        trainImages.put(rawTrainingData[j].toFloat() / 255.0f)
                    else
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
        return BatchedDataset(trainInputBatches, trainLabelBatches, batchSize)
    }

    //unused I think
    fun dataBatchIterator(
        batchSize: Int,
        data_percent: Float,
        normalize: Boolean
    ): Cifar10DataBatchIterator =
        Cifar10DataBatchIterator(assetManager, data_percent, normalize).batch(batchSize)

    override fun datasetIterator(): Cifar10DataBatchIterator =
        Cifar10DataBatchIterator(assetManager, dataPercent, normalize).batch(batchSize)

    //Could make outer but meh
    class Cifar10DataBatchIterator(
        private val assetManager: AssetManager,
        val dataPercent: Float = 1.0f,
        val normalize: Boolean = false
    ) :
        DatasetIterator<FloatBuffer, LongBuffer> {

        var hasStarted = false
            private set

        private var batchSize: Int = BATCH_SIZE

        //AKA number of images
        private val totalData: Int = (TRAINING_DATA_COUNT * dataPercent).toInt()
        //^^ Note this adds complexity of final batch as size will be smaller

        //VV Actually may not need, I really am tired and hungry!!
        // VV Will need changing in batch fun
        //private val numberOfBatches: Int = if ((totalData % batch) == 0)
        //    (totalData / batch) else (totalData / batch) + 1

        private var amountRead: Int = 0
        private var wholeFilesRead: Int = 0
        private var readBuffer = ByteArray(BATCH_FILE_SIZE) //AKA cache!
        private var bufferOffset: Int = 0


        fun batch(batchSize: Int): Cifar10DataBatchIterator {
            if (!hasStarted)
                this.batchSize = batchSize
            return this
        }

        override fun hasNext(): Boolean = amountRead < totalData

        override fun reset(): Cifar10DataBatchIterator {
            hasStarted = false
            amountRead = 0
            wholeFilesRead = 0
            bufferOffset = 0

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


            val trainBatch = directAllocateNativeFloatBuffer(batchSize * INPUT_SIZE)
            val labelBatch = directAllocateNativeLongBuffer(batchSize * LABEL_SIZE)

            for (i in 0 until batchSize) {

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
            return DataBatch(trainBatch, labelBatch, batchSize)
        }
    }

}