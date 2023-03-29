package com.calotechnologies.faldroid.dataset

import com.calotechnologies.faldroid.dataset.iterator.DatasetIterator

typealias TrainingTestPair<InputBuffer, LabelBuffer> =
        Pair<DatasetIterator<InputBuffer, LabelBuffer>, DatasetIterator<InputBuffer, LabelBuffer>>

//TODO: maybe change lists to arrays for interop sake
interface Dataset<InputBuffer, LabelBuffer> {

    fun iterator(): TrainingTestPair<InputBuffer, LabelBuffer>

}