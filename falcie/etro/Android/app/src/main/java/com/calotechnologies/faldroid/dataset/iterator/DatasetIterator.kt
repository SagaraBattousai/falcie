package com.calotechnologies.faldroid.dataset.iterator

import com.calotechnologies.faldroid.dataset.DataBatch
import java.nio.FloatBuffer
import java.nio.LongBuffer

interface DatasetIterator<InputBuffer, LabelBuffer> {

    val length: Int

    operator fun hasNext(): Boolean
    operator fun next(): DataBatch<InputBuffer, LabelBuffer>

    fun batch(batchSize: Int): DatasetIterator<InputBuffer, LabelBuffer>
    fun reset(): DatasetIterator<InputBuffer, LabelBuffer>


}