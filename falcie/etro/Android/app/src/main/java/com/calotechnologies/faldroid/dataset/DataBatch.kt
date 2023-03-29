package com.calotechnologies.faldroid.dataset

data class DataBatch<InputBuffer, LabelBuffer>(
    val inputBatch: InputBuffer,
    val labelBatch: LabelBuffer,
    val batchSize: Int
)
{
    //Alis'
    val x: InputBuffer get() = inputBatch
    val y: LabelBuffer get() = labelBatch
    val length: Int get() = batchSize
}
