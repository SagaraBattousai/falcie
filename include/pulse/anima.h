#ifndef __PULSE_ANIMA_ANIMA_H__
#define __PULSE_ANIMA_ANIMA_H__

#include <stdint.h>

typedef float (*activation_t)(float);
typedef float (*delta_activation_t)(float);

typedef struct neural_network
{ 
    /** @c input_length: The length of the input array. */
    int64_t input_size;
    /** @c input: A pointer to the networks inputs. */
    float *input;
    /**
     * @c num_layers: The number of neuron / weight layers in the network,
     * i.e. the total number of layers excluding the input layer (also equal
     * to the dimensions of the weight array).
     */
    int64_t num_layers;
    /**
     * @c neuron_dims is an array describing the shape
     * of the neurons in each layer. The array must be of length @c num_layers
     * and structured such that @c neuron_dims[layer - 1] is the number of neurons
     * at layer @c layer
     */
    int64_t *neuron_dims;
    /**
    * @c weights: An array of weights with @c num_layers dimensions each of
    * which are described by @c neuron_dims. The true dimensions of weights, 
    * since it is a 2D array, 
    * is @c [neuron_dims[layer1 - 1] * neuron_dims[layer2 -1]], such that 
    * the weight connects layer1 to layer2
    */
    float **weights;

    //TODO: make into an activation array.
    /**
    * @c activation: A function pointer to the
    * activation function for the network.
    */
    activation_t activation;

    delta_activation_t delta_activation;

    /** @c learning_rate: The learning rate of the network. */
    float learning_rate;
} neural_network_t;

typedef struct feedforward_tracking
{
    /** @c layers: A matrix of the layers through the network. */
    float **layers;
    /**
     * @c pre_act_layers: A matrix of the layers, before the activation
     * is applied, through the network.
     */
    float **pre_activation_layers;

    /** @c total_layer_count: The number of layers (including the
     * input layer). Used primarily to help with freeing memory.
     */
    int64_t total_layer_count;

}feedforward_tracking_t;

//Note my weight dims are swiched from Benny's example.
/*****************************************************************************
* Neural Network feed forward.
* 
* @param[in] network The network to perform the forward pass on.
* @param[out] tracking Records the layers and and pre activation layers
* through a forward pass of the network. This can then be used to calculate
* the gradients via backpropagation.
******************************************************************************/
int feedforward(neural_network_t *network, feedforward_tracking_t *tracking);

int backpropagation(float ***new_weights, float *desired, neural_network_t *network, feedforward_tracking_t *tracking);

void free_tracking(feedforward_tracking_t *tracking);

//TODO: Hide this later
int copy_weights(float ***dst, float**src, int64_t num_layers, int64_t input_dim, int64_t *neuron_dims);

#endif