package com.calotechnologies.faldroid.dataset

import android.content.res.AssetManager
import com.calotechnologies.faldroid.dataset.cifar10.Cifar10Dataset

abstract class Dataset<InputBuffer, LabelBuffer> {

    //Single Datapoint
    inner class Datapoint(private val input: InputBuffer,
                          private val label: LabelBuffer)
    {
        operator fun component1() = input
        operator fun component2() = label
    }

    //A Batch of Data
    inner class DataBatch(val inputBatch: InputBuffer,
                          val labelBatch: LabelBuffer,
                          val batchSize: Int)
    {
        operator fun component1() = inputBatch
        operator fun component2() = labelBatch
    }

    inner class BatchedDataset(val inputBatch: List<InputBuffer>,
                               val labelBatch: List<LabelBuffer>,
                               val batchSize: Int)
    {
        val batches = inputBatch.size

        operator fun component1() = inputBatch
        operator fun component2() = labelBatch

        //COMPONENT3 ???
    }

    abstract fun DataBatchIterator(batchSize: Int,
                                   data_percent: Float = 1.0f,
                                   normalize: Boolean = true) : Iterator<DataBatch>

}