package com.calotechnologies.faldroid.model

import org.junit.Assert
import org.junit.Test

private const val LAYER_1_KERNEL_AND_BIAS_PARAMETERS = (3 * 3 * 3 * 32) + 32

class ModelTests {

    @Test
    fun weights_can_be_flattened_an_restored() {
        val weights = Model.newWeightBuffer()
        for (w in weights) {
            for (i in 0 until w.floatBuffer.capacity())
                w.floatBuffer.put(i.toFloat())
                w.floatBuffer.rewind()
        }
        //Would be cooler if code could do this by ref but probably harder to test!
        val flattenedWeights = Model.flattenWeightBuffer(weights)

        Assert.assertEquals(
            "Second element of second kernel (third weight layer) = corresponding in flattened",
            weights[2][1], flattenedWeights[LAYER_1_KERNEL_AND_BIAS_PARAMETERS + 1]
        )

        val reshapedWeights = Model.reformWeightBuffer(flattenedWeights)

        for(wi in reshapedWeights.indices) {
            Assert.assertArrayEquals(reshapedWeights[wi].array, weights[wi].array)
        }
    }

}