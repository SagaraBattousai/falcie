#ifndef __PULSE_ANIMA_H__
#define __PULSE_ANIMA_H__

#include <stdint.h>

/*************************************************************
**************** Anima's Public API Header *******************
**************************************************************/

typedef float (*activation_t)(float);

typedef float (*error_energy)(float *, float *, int64_t);

typedef struct neural_network neural_network_t;

//TODO: TODO: TODO:
/**********JUST FOR NOW MUST! MUST MUST Move back later */
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

} feedforward_tracking_t;


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

    activation_t delta_activation;

    /** @c learning_rate: The learning rate of the network. */
    float learning_rate;

    feedforward_tracking_t *tracking;


} neural_network_t;

/***********************************************************/

//TODO: Clean this up!!
neural_network_t *new_neural_network(int64_t input_size, int64_t num_layers,
	int64_t *neuron_dims, activation_t activation,
	activation_t delta_activation, float learning_rate, unsigned* seed);

void set_input(neural_network_t *network, float *input);

void get_weight(neural_network_t *network, float ***weights);

void set_weight(neural_network_t *network, float **weights);

void update_weight(neural_network_t *network, float** deltaweights);

void get_network_output(neural_network_t *network, float **output);

//feedforward_tracking_t *new_feedforward_tracking();

/*****************************************************************************
* Neural Network feed forward.
*
* @param[in] network The network to perform the forward pass on.
* @param[out] tracking Records the layers and and pre activation layers
* through a forward pass of the network. This can then be used to calculate
* the gradients via backpropagation.
******************************************************************************/
int feedforward(neural_network_t *network);

//Although it seems like a waste now, i think extracting delta weights was infact a good idea
// Hopefully it will allow for ofloading back prop training too.
//Maybe make **weights a struct. then just ptr to that struct.
int backpropagation(float ***delta_weights, float *desired, neural_network_t *network);

// batch has size batch size and batch[x] has input size size
int train_network(float **desired, float **batch, int64_t batch_size, int64_t epochs, neural_network_t *network);

// batch has size batch size and batch[x] has input size size
int train_network_with_stats(float **desired, float **batch, int64_t batch_size,
    int64_t epochs, neural_network_t *network, int64_t *epochs_taken, float *avg_err, float target_err, error_energy err_func);


void free_neural_network(neural_network_t *network);

void print_inferencing_results(neural_network_t *network,
	float **training_data, int64_t training_data_size, int64_t training_data_dimension,
	float **desired, int64_t desired_dimension);

//TODO: AVX implemntation
void vector_vector_sum(float *vector1, float *vector2, float *out, const int64_t dim);

//TODO: AVX implemntation
void vector_vector_sub(float *vector1, float *vector2, float *out, const int64_t dim);

//TODO: AVX implemntation
void scalar_vector_sum(const float scalar, float *vector, float *out, const int64_t dim);

//TODO: AVX implemntation
void scalar_vector_mult(const float scalar, float *vector, float *out, const int64_t dim);


float vector_dot(float *in1, float *in2, const int64_t dim);


#endif