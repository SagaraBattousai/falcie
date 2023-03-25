package com.calotechnologies.faldroid.dataset

abstract class Dataset<InputBuffer, LabelBuffer> {

    inner class Datapoint(val input: InputBuffer, val label: LabelBuffer)
    {
        operator fun component1() = input
        operator fun component2() = label
    }

    inner class DataBatch(val inputBatch: List<InputBuffer>, val labelBatch: List<LabelBuffer>)
    {
        operator fun component1() = inputBatch
        operator fun component2() = labelBatch
    }

}