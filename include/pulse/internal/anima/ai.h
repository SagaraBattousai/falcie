#ifndef __PULSE_ANIMA_AI_H__
#define __PULSE_ANIMA_AI_H__

#include <stdint.h>

#include <pulse/anima.h>




void rand_weights(float ***weight_ptr, int64_t no_layers, int64_t no_inputs, int64_t *nd, unsigned *seed);

//TODO: Hide this later
int copy_weights(float ***dst, float **src, int64_t num_layers, int64_t input_dim, int64_t *neuron_dims);

//TODO: Make this better!
float frand(unsigned int *seed);

void free_tracking(feedforward_tracking_t *tracking);



#endif