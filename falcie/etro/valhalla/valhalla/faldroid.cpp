#include <jni.h>
#include <string>
#include <cstring>
#include <vector> //Safe to use with JNI?????
#include <span> //^^ Same question Slightly more dangerous
#include <utility> //^^ Again
#include <cstdint>


#include <cactuar/block.h>
#include <cactuar/blockchain.h>
#include <cactuar/network_structure_update.h>


using Block_t = cactuar::Block;
using Builder_t = cactuar::Block::Builder;
using Blockchain_t = cactuar::Blockchain;
using NetworkStructureUpdate_t = cactuar::NetworkStructureUpdate;

extern "C"
JNIEXPORT jlong JNICALL
Java_com_calotechnologies_faldroid_blockchain_Blockchain_createBuilder(JNIEnv *env, jobject thiz,
                                                                       jint version, jint target)
{
  Builder_t *builder = new Builder_t();
  builder->WithVersion(version).WithTarget(target);

  return reinterpret_cast<jlong>(builder);
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_calotechnologies_faldroid_blockchain_Blockchain_createBlockchain(JNIEnv *env, jobject thiz)
{
  return reinterpret_cast<jlong>(new Blockchain_t());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_calotechnologies_faldroid_blockchain_Blockchain_federate(
    JNIEnv *env, jobject thiz, jlong blockchain_ptr, jlong block_builder,
    jobjectArray model_weight_layers, jintArray model_examples_seen,
    jobjectArray federated_weights) {
  Builder_t *builder = reinterpret_cast<Builder_t *>(block_builder);
  Block_t block = builder->Build();

  jsize participating_models = env->GetArrayLength(model_weight_layers);

  // NetworkStructureUpdate_t **network_updates =
  //     new NetworkStructureUpdate_t *[participating_models];
  // std::vector<NetworkStructureUpdate_t> network_updates{
  //     (std::vector<NetworkStructureUpdate_t>::size_type) participating_models
  // }; //Do we need to initialise each elem?

  jsize num_layers;  // Should be identical for all models
  jobjectArray weight_layers;
  jint *examples_seen = env->GetIntArrayElements(model_examples_seen, nullptr);

  for (jsize model_index = 0; model_index < participating_models;
       ++model_index) {
    // weight_layers has Kotlin Type Array<FloatBuffer>
    weight_layers =
        (jobjectArray)  // Technically redundant but I like to be clear it's 2D
        env->GetObjectArrayElement(model_weight_layers, model_index);

    num_layers = env->GetArrayLength(weight_layers);

    // TODO: maybe do something to allow constructing with capacity for
    // delta_weights
    NetworkStructureUpdate_t network_update{};

    for (jsize layer_index = 0; layer_index < num_layers; ++layer_index) {
      // buffer has Kotlin Type FloatBuffer
      jobject buffer = env->GetObjectArrayElement(weight_layers, layer_index);
      float *weights = (float *)env->GetDirectBufferAddress(buffer);
      jlong weight_length = env->GetDirectBufferCapacity(buffer);

      // NOTE: For now we will treat each weight layer as a vector and then
      // optimise from there! update->delta_weights.emplace_back
      // network_updates[model_index].delta_weights.emplace_back(
      //        std::vector<float>(weights, weights + weight_length)
      //            );
      // TODO: Undo
      /*
       network_update.delta_weights.emplace_back(
          std::vector<float>(weights, weights + weight_length)
      );
       */
    }

    network_update.examples_seen = examples_seen[model_index];
    block.AddLocalUpdate(network_update);
  }
  env->ReleaseIntArrayElements(model_examples_seen, examples_seen, JNI_ABORT);

  Blockchain_t *blockchain = reinterpret_cast<Blockchain_t *>(blockchain_ptr);
  blockchain->Add(std::move(block));

  NetworkStructureUpdate_t global_update =
      blockchain->GetLast().GetGlobalUpdate();

  jsize gu_layer_count = env->GetArrayLength(federated_weights);
  for (jsize gu_layer_index = 0; gu_layer_index < gu_layer_count;
       ++gu_layer_index) {
    jobject gu_weight_buffer =
        env->GetObjectArrayElement(federated_weights, gu_layer_index);

    float *gu_weights = (float *)env->GetDirectBufferAddress(gu_weight_buffer);
    jlong gu_weights_length = env->GetDirectBufferCapacity(gu_weight_buffer);

    // TODO: UNDO!!!
    // memcpy(gu_weights, global_update.delta_weights[gu_layer_index].Data(),
    // gu_weights_length * sizeof(float));
  }
}























