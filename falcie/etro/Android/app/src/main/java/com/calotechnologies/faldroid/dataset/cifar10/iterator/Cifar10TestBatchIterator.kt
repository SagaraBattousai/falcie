package com.calotechnologies.faldroid.dataset.cifar10.iterator

import android.content.res.AssetManager
import com.calotechnologies.faldroid.dataset.DataBatch
import com.calotechnologies.faldroid.dataset.cifar10.Cifar10Dataset
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeLongBuffer
import java.nio.FloatBuffer
import java.nio.LongBuffer

//Can we have internal constructor?

//TODO: Make cleaner interface messy af but today has been painfully unproductive
//E.G. Could make batchSize lateinit etc but ... this is hard
class Cifar10TestBatchIterator (
    assetManager: AssetManager,
    dataPercent: Float = 1.0f,
    initialBatchSize: Int = Cifar10Dataset.BATCH_SIZE,
    normalize: Boolean = false
) : Cifar10DataBatchIterator((Cifar10Dataset.TRAINING_DATA_COUNT * dataPercent).toInt(), initialBatchSize, normalize) {

    companion object { const val TAG = "CIFAR10_TEST_BATCH_ITERATOR_TAG:" }

    private var batchIndex: Int = 0
    private var amountRead: Int = 0
    private val readBuffer = ByteArray(Cifar10Dataset.BATCH_FILE_SIZE)

    init {
        assetManager.open(Cifar10Dataset.testDatasetFilename).use {
            it.read(readBuffer, 0, Cifar10Dataset.BATCH_FILE_SIZE)
        }
    }

    private var bufferOffset: Int = 0

    override fun hasNext(): Boolean = amountRead < length

    override fun reset(): Cifar10TestBatchIterator {
        hasStarted = false
        amountRead = 0
        bufferOffset = 0

        return this
    }

    override fun next(): DataBatch<FloatBuffer, LongBuffer> {

        if (!hasNext())
            throw NoSuchElementException("Iterator Has Finished")

        hasStarted = true

        val currentBatchSize = batchSplits[batchIndex]

        val trainBatch = directAllocateNativeFloatBuffer(currentBatchSize * Cifar10Dataset.INPUT_SIZE)
        val labelBatch = directAllocateNativeLongBuffer(currentBatchSize * Cifar10Dataset.LABEL_SIZE)

        for (i in 0 until currentBatchSize) {

            readDataPoint(trainBatch, labelBatch, readBuffer, bufferOffset)

            bufferOffset += Cifar10Dataset.DATA_SIZE // Cifar10Dataset.INPUT_SIZE

            ++amountRead
        }

        trainBatch.rewind()
        labelBatch.rewind()

        return DataBatch(trainBatch, labelBatch, currentBatchSize)
    }
}
