#ifndef __PULSE_ANIMA_H__
#define __PULSE_ANIMA_H__

#include <stdint.h>

/*************************************************************
**************** Anima's Public API Header *******************
**************************************************************/

typedef float (*activation_t)(float);

typedef struct neural_network neural_network_t;

//TODO: Clean this up!!
neural_network_t *new_neural_network(int64_t input_size, int64_t num_layers,
	int64_t *neuron_dims, activation_t activation,
	activation_t delta_activation, float learning_rate, unsigned* seed);

void set_input(neural_network_t *network, float *input);

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

//Maybe make **weights a struct. then just ptr to that struct.
int backpropagation(float ***new_weights, float *desired, neural_network_t *network);

void free_neural_network(neural_network_t *network);



void vector_vector_sub(float *vector1, float *vector2, float *out, const int64_t dim);

float vector_dot(float *in1, float *in2, const int64_t dim);


#endif