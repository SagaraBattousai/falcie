package com.calotechnologies.faldroid.dataset

//TODO: maybe change lists to arrays for interop sake
interface Dataset<InputBuffer, LabelBuffer> : Iterable<DataBatch<InputBuffer, LabelBuffer>> {

    //Couldn't give it a different name and have normal iterator = this (under sep name)
    // JVM interop misunderstanding (although worked before so must have something to do with test?
    override fun iterator(): DatasetIterator<InputBuffer, LabelBuffer>

}