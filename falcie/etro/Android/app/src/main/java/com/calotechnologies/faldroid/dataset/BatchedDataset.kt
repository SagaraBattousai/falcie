package com.calotechnologies.faldroid.dataset

data class BatchedDataset<InputBuffer, LabelBuffer>(
    val batchedInputs: List<InputBuffer>,
    val batchedLabels: List<LabelBuffer>,
    val batchSize: Int
) {
    val batches = batchedInputs.size
}
