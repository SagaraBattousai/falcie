package com.calotechnologies.faldroid.dataset

interface DatasetIterator<InputBuffer, LabelBuffer> :
    Iterator<DataBatch<InputBuffer, LabelBuffer>> {

    fun reset(): DatasetIterator<InputBuffer, LabelBuffer>

}