package com.calotechnologies.faldroid.dataset

//Single Datapoint
data class Datapoint<InputBuffer, LabelBuffer>(
    val input: InputBuffer,
    val label: LabelBuffer
)