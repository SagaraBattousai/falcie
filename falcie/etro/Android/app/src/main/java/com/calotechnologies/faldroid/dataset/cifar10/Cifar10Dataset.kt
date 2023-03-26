package com.calotechnologies.faldroid.dataset.cifar10

import android.content.res.AssetManager
import android.util.Log
import com.calotechnologies.faldroid.dataset.Dataset
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeLongBuffer
import java.io.InputStream
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import java.nio.LongBuffer

class Cifar10Dataset(private val assetManager: AssetManager) : Dataset<FloatBuffer, LongBuffer>() {

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
    fun getDatapoint(
        assetManager: AssetManager,
        datapoint: Int,
        normalize: Boolean = true
    ): Datapoint {
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
        val label: LongBuffer = ByteBuffer
            .allocateDirect(LABEL_SIZE * Long.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asLongBuffer()

        label.put(rawData[0].toLong())

        //FloatBuffer.allocate(INPUT_SIZE)
        val pixels: FloatBuffer = ByteBuffer
            .allocateDirect(INPUT_SIZE * Float.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asFloatBuffer()

        for (i in 1..INPUT_SIZE) {
            if (normalize)
                pixels.put(rawData[i].toFloat() / 255.0f)
            else
                pixels.put(rawData[i].toFloat())
        }

        return Datapoint(pixels, label)
    }

    //Pre BatchSize must be a multiple of 50,000 AKA Training Data Size
    fun getTrainingData(
        assetManager: AssetManager,
        batchSize: Int = BATCH_SIZE,
        normalize: Boolean = true
    ): BatchedDataset {

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

    override fun DataBatchIterator(
        batchSize: Int,
        data_percent: Float,
        normalize: Boolean
    ): Cifar10DataBatchIterator {
        return Cifar10DataBatchIterator(batchSize, data_percent, normalize)
    }

    inner class Cifar10DataBatchIterator(
        val batchSize: Int,
        val data_percent: Float,
        val normalize: Boolean
    ) : Iterator<DataBatch> {

        private var amountRead: Int = 0
        private var wholeFilesRead: Int = 0
        private var readBuffer = ByteArray(BATCH_FILE_SIZE) //AKA cache!
        private var bufferOffset: Int = 0


        //AKA number of images
        private val totalData: Int = (TRAINING_DATA_COUNT * data_percent).toInt()

        //^^ Note this adds complexity of final batch as size will be smaller

        //VV Actually may not need, I really am tired and hungry!!
        private val numberOfBatches: Int = if ((totalData % batchSize) == 0)
            (totalData / batchSize) else (totalData / batchSize) + 1

        init {
            readNextChunk()
        }

        override fun hasNext(): Boolean = amountRead < totalData

        private fun readNextChunk() {
            val inputStream = assetManager.open(trainingDatasetFilenames[wholeFilesRead])
            inputStream.read(readBuffer, 0, BATCH_FILE_SIZE)
            inputStream.close()
        }

        override fun next(): DataBatch {
            //assert wholeFilesRead < 5

            val trainBatch = directAllocateNativeFloatBuffer(batchSize * INPUT_SIZE)
            val labelBatch = directAllocateNativeLongBuffer(batchSize * LABEL_SIZE)

            for (i in 0 until batchSize)
            {
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
                if (bufferOffset == BATCH_FILE_SIZE)
                {
                    bufferOffset = 0
                    ++wholeFilesRead
                    readNextChunk()
                }
            }
            trainBatch.rewind()
            labelBatch.rewind()
            return DataBatch(trainBatch, labelBatch, batchSize)
        }
    }

}