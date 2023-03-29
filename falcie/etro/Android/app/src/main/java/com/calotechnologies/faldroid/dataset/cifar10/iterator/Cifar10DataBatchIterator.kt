package com.calotechnologies.faldroid.dataset.cifar10.iterator

import android.util.Log
import com.calotechnologies.faldroid.dataset.iterator.DatasetIterator
import com.calotechnologies.faldroid.dataset.cifar10.Cifar10Dataset
import com.calotechnologies.faldroid.utils.Math
import java.nio.FloatBuffer
import java.nio.LongBuffer

//Currently poor design but we'll (possibly) get there
abstract class Cifar10DataBatchIterator(
    //private val assetManager: AssetManager,
    //dataPercent: Float = 1.0f,
    initialBatchSize: Int = Cifar10Dataset.BATCH_SIZE,
    val normalize: Boolean = false
) : DatasetIterator<FloatBuffer, LongBuffer> {

    companion object {
        const val TAG = "CIFAR10_DATA_BATCH_ITERATOR_TAG:"
    }

    var hasStarted = false
        protected set

    var batchSize: Int = initialBatchSize
        protected set

    var numberOfBatches: Int = recalculateNumberOfBatches()
        protected set

    var batchSplits: IntArray = recalculateBatchSplits()
        protected set

    //cant call get or set because of jvm interop
    protected fun recalculateNumberOfBatches(): Int = Math.ceilDiv(length, batchSize)

    //cant call get or set because of jvm interop
    protected fun recalculateBatchSplits(): IntArray {
        val splits = IntArray(numberOfBatches) { batchSize }

        val remainingSplit = length % batchSize
        if (remainingSplit != 0)
            splits[numberOfBatches - 1] = remainingSplit

        return splits
    }

    override fun batch(batchSize: Int): Cifar10DataBatchIterator {
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

    protected fun readDataPoint(
        inputBatch: FloatBuffer,
        labelBatch: LongBuffer,
        readBuffer: ByteArray,
        bufferOffset: Int
    ) {

        labelBatch.put(readBuffer[bufferOffset].toLong())
        val inputOffset = bufferOffset + 1

        for (j in 0 until Cifar10Dataset.INPUT_SIZE) {
            var pixelChannel: Float = readBuffer[j + inputOffset].toFloat()
            if (normalize)
                pixelChannel /= 255.0f
            inputBatch.put(pixelChannel)
        }
    }


}
