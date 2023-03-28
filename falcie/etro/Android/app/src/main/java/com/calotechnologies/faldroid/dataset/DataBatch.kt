package com.calotechnologies.faldroid.dataset

data class DataBatch<InputBuffer, LabelBuffer>(
    val inputBatch: InputBuffer,
    val labelBatch: LabelBuffer,
    val batchSize: Int
)
