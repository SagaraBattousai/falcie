package com.calotechnologies.faldroid.blockchain

import com.calotechnologies.faldroid.model.FederatedWeights
import com.calotechnologies.faldroid.model.Model
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import java.nio.FloatBuffer

//Long as 64bits in order to avoid issues with 64 or 32 bit native pointers
private typealias Ptr = Long

private fun isNull(ptr: Ptr): Boolean = (ptr == 0L)

class Blockchain {

    //Static init so well shove everything into this file to make life easier for now!
    companion object {
        init {
            System.loadLibrary("faldroid")
        }

        private const val BLOCK_VERSION: Int = 0x01
        private const val BLOCK_TARGET: Int = 0x21FFFFFF
        //private const val BLOCKCHAIN_UNROLL: Int = 32
    }

    private var blockBuilderPtr: Ptr = 0
    private var blockchainPtr: Ptr = 0

    init {
        blockBuilderPtr = createBuilder(BLOCK_VERSION, BLOCK_TARGET)
        blockchainPtr = createBlockchain()
    }

    //TODO: Make sure models contains number of examples seen
    //Also cant remember that much as it came to me as I fell asleep lats night,
    //got to A handle different epochs used in training and B handle over fitting etc in mining
    //possibly by training and or by testing
    fun federate(
        modelWeights: Array<Array<FloatBuffer>>, examplesSeen: LongArray
    ): Array<FloatBuffer> {
        val federatedWeights = Model.newWeightBuffer()

        federate(blockchainPtr, blockBuilderPtr, modelWeights, examplesSeen, federatedWeights)

        return federatedWeights
    }

    fun federate(
        modelWeights: Array<Array<FloatBuffer>>, examplesSeen: LongArray,
        federatedWeights: Array<FloatBuffer>
    ) =
        federate(blockchainPtr, blockBuilderPtr, modelWeights, examplesSeen, federatedWeights)

    private external fun createBuilder(version: Int, target: Int): Ptr
    private external fun createBlockchain(): Ptr

    //I think we can get away with one final native method and one normal method that calls it :)
    private external fun federate(
        blockchainPtr: Ptr,
        blockBuilder: Ptr,
        modelWeightLayers: Array<Array<FloatBuffer>>,
        modelExamplesSeen: LongArray,
        federatedWeights: Array<FloatBuffer>,
    )


/*
bb = falpy.BlockBuilder(0x01, 0x21FFFFFF)
bc = falpy.Blockchain(bb, 32)

model_examples_seen = [0] * num_models
#####################


batch_splits = [math.floor(bs * len(ds_train_batch) / num_models) \
                for bs in range(1, num_models + 1) ]

for epoch in range(num_epochs):
  for i, (x, y) in enumerate(ds_train_batch):

    if i >= batch_splits[model_index]:
      model_index += 1

    #Train Model....................
    #Update examples seen for this model
    model_examples_seen[model_index] += len(x)

    #Generate metrics for model 1 (including examples it hasn't seen)..................

  ### Falpy ###
  if True: #epoch != 0 and epoch % epoch_blockchain_submission == 0:

    block = bb.build()

    #network_updates = [falpy.NetworkUpdate()] * num_models # Seems to make refs
    #^^ Makes num_model references to the same network update!! :(
    network_updates = [falpy.NetworkUpdate() for _ in range(num_models)]

    #For now lets cheat and assume every variable is trainable
    #Also may not be a cheat since it is possible that
    #if all not trainable vars are static then it will average to itself

    for i, (model, _) in enumerate(models):
      for weight in model.get_weights():
        network_updates[i].add_weight(falpy.Weights(weight))
        network_updates[i].set_examples_seen(model_examples_seen[i])

    for i in range(len(network_updates)):
      block.add_local_update(network_updates[i])

    bc.add(block) ##Being moved not copied, could cause issues
    # last = bc.get_last()
    gu = bc.get_last().get_global_update()

    #TODO: This (Hopefully) Is the last thing to fix!
    #Much like other bugs gu.get_weights MUST be assigned for some
    #(as yet) unknown reason!
    throw = gu.get_weights()

    blockchain_weights = [w.g_array for w in throw]

    for model, _ in models:
      model.set_weights(blockchain_weights)

    model_examples_seen = [0] * num_models

  if epoch % 50 == 0:
    print("Epoch {:03d}: Loss: {:.3f}, Accuracy: {:.3%}".format
          (epoch,
           epoch_loss_avg.result(),
           epoch_accuracy.result()))


print("Training Finished:")

 */
}