package com.calotechnologies.faldroid.model

//Long as std::int64_t in C/C++
data class FederatedWeights<W>(val weights: Array<W>, val examplesSeen: Long)
