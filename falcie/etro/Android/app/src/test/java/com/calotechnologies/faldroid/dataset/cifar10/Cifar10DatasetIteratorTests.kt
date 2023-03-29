package com.calotechnologies.faldroid.dataset.cifar10

import android.content.res.AssetManager
import org.junit.Assert
import org.junit.Test
import org.junit.runner.RunWith
import org.mockito.junit.MockitoJUnitRunner
import org.mockito.kotlin.mock
import kotlin.math.ceil

@RunWith(MockitoJUnitRunner::class)
class Cifar10DatasetIteratorTests {

    //@Mock
    //private lateinit var mockAssetManager: AssetManager

    //Note: message names were so similar I got confused debugging!! Silly Me!!
    @Test
    fun numberOfBatchesAndBatchSplitsUpdate() {
        val mockAssetManager = mock<AssetManager> {}
        val dataPercent: Float = 0.25f
        val expectedLength: Int =
            (dataPercent * Cifar10Dataset.TRAINING_DATA_COUNT).toInt() //12,500
        val iterator = Cifar10Dataset.Cifar10DataBatchIterator(mockAssetManager, dataPercent)

        Assert.assertEquals("Expected Iterator Length", expectedLength, iterator.length)

        val originalExpectedNumberOfBatches: Int =
            ceil(expectedLength.toFloat() / Cifar10Dataset.BATCH_SIZE).toInt() //2

        Assert.assertEquals(
            "Expected default number of batches for 25% of data",
            originalExpectedNumberOfBatches,
            iterator.numberOfBatches
        )

        val originalExpectedBatchSplits: IntArray = intArrayOf(
            Cifar10Dataset.BATCH_SIZE,
            expectedLength % Cifar10Dataset.BATCH_SIZE
        )

        Assert.assertEquals(
            "Number of batchSplits = NumberOfBatches",
            originalExpectedNumberOfBatches,
            iterator.batchSplits.size
        )

        Assert.assertArrayEquals(
            "Expected default batchSplits for 25% of data",
            originalExpectedBatchSplits,
            iterator.batchSplits
        )

        //Change batch size
        val newBatchSize = 1785 //i.e. 8 splits with the last split being tiny
        iterator.batch(newBatchSize)

        Assert.assertEquals(
            "Iterator Length Doesn't change on batching",
            expectedLength,
            iterator.length
        )

        val expectedNewNumberOfBatches: Int =
            ceil(expectedLength.toFloat() / newBatchSize).toInt() //8

        Assert.assertEquals(
            "Expected new number of batches for 25% of data",
            expectedNewNumberOfBatches,
            iterator.numberOfBatches
        )

        val newExpectedBatchSplits: IntArray = intArrayOf(
            newBatchSize, newBatchSize, newBatchSize, newBatchSize,
            newBatchSize, newBatchSize, newBatchSize, expectedLength % newBatchSize,
        )

        Assert.assertEquals(
            "Number of batchSplits = NumberOfBatches",
            expectedNewNumberOfBatches,
            iterator.batchSplits.size
        )

        Assert.assertArrayEquals(
            "Expected new batchSplits for 25% of data",
            newExpectedBatchSplits,
            iterator.batchSplits
        )


    }
}