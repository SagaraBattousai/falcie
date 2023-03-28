package com.calotechnologies.faldroid.dataset

//TODO: maybe change lists to arrays for interop sake
interface Dataset<InputBuffer, LabelBuffer> : Iterable<DataBatch<InputBuffer, LabelBuffer>> {

    override fun iterator(): Iterator<DataBatch<InputBuffer, LabelBuffer>> = datasetIterator()

    fun datasetIterator(): DatasetIterator<InputBuffer, LabelBuffer>

}