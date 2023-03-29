package com.calotechnologies.faldroid.dataset.cifar10.iterator

import android.content.res.AssetManager
import android.util.Log
import com.calotechnologies.faldroid.dataset.DataBatch
import com.calotechnologies.faldroid.dataset.cifar10.Cifar10Dataset
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeLongBuffer
import java.nio.FloatBuffer
import java.nio.LongBuffer

//Can we have internal constructor?

//TODO: Make cleaner interface messy af but today has been painfully unproductive
//E.G. Could make batchSize lateinit etc but ... this is hard
class Cifar10TrainingBatchIterator (
    private val assetManager: AssetManager,
    dataPercent: Float = 1.0f,
    initialBatchSize: Int = Cifar10Dataset.BATCH_SIZE,
    normalize: Boolean = false
) : Cifar10DataBatchIterator(initialBatchSize, normalize) {

    companion object { const val TAG = "CIFAR10_TRAINING_BATCH_ITERATOR_TAG:" }

    //AKA number of images
    override val length: Int = (Cifar10Dataset.TRAINING_DATA_COUNT * dataPercent).toInt()

    private var currentBatchIndex: Int = 0
    private var amountRead: Int = 0
    private var wholeFilesRead: Int = 0
    private var readBuffer = ByteArray(Cifar10Dataset.BATCH_FILE_SIZE) //AKA cache!
    private var bufferOffset: Int = 0

    override fun hasNext(): Boolean = amountRead < length

    override fun reset(): Cifar10TrainingBatchIterator {
        hasStarted = false
        amountRead = 0
        wholeFilesRead = 0
        bufferOffset = 0
        currentBatchIndex = 0

        return this
    }

    private fun readNextChunk() {
        val inputStream = assetManager.open(Cifar10Dataset.trainingDatasetFilenames[wholeFilesRead])
        inputStream.read(readBuffer, 0, Cifar10Dataset.BATCH_FILE_SIZE)
        inputStream.close()
    }

    override fun next(): DataBatch<FloatBuffer, LongBuffer> {

        if (!hasNext())
            throw NoSuchElementException("Iterator Has Finished")

        hasStarted = true

        val currentBatchSize = batchSplits[currentBatchIndex]

        val trainBatch = directAllocateNativeFloatBuffer(currentBatchSize * Cifar10Dataset.INPUT_SIZE)
        val labelBatch = directAllocateNativeLongBuffer(currentBatchSize * Cifar10Dataset.LABEL_SIZE)

        for (i in 0 until currentBatchSize) {

            if (bufferOffset == 0)
                readNextChunk()

            readDataPoint(trainBatch, labelBatch, readBuffer, bufferOffset)

            bufferOffset += Cifar10Dataset.DATA_SIZE // Cifar10Dataset.INPUT_SIZE

            ++amountRead

            if (bufferOffset == Cifar10Dataset.BATCH_FILE_SIZE) {
                bufferOffset = 0
                ++wholeFilesRead
            }
        }

        trainBatch.rewind()
        labelBatch.rewind()

        ++currentBatchIndex
        return DataBatch(trainBatch, labelBatch, currentBatchSize)
    }
}
