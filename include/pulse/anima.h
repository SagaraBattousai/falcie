#ifndef __PULSE_ANIMA_ANIMA_H__
#define __PULSE_ANIMA_ANIMA_H__

typedef float (*activation_t)(float);
/*-----------------------------------------------------------------
Note: Messy af and fiddly to delete memory as input is also pointed
to by both layers and pre_act_layers. Starting to see why we should
use smart pointers but alas, for now we shall ignore!

TODO: Replace with structs!
------------------------------------------------------------------*/


//-------------------------------------------------
// Usually I hate returning something I've malloced in a func.
// However, I think this time it's somewhat justified.
/*****************************************************************************
* Neural Network feed forward.
*
*
* @param[in]  input          is a pointer to the networks inputs.
* @param[in]  weights        is an array of weights with **num_layers** 
* dimensions and described by weight_dims. 
* @param[in]  activation     is a function pointer to the activation 
* function for the network.
* TODO: make into an activation array.
* @param[in, out] layers         will be a matrix of the layers through the 
* network. Must be of size num_layers + 1.
* Don't forget to free once you're done.
* @param[in, out] pre_act_layers will be a matrix of the layers, before the
* activation is applied, through the network. Must be of size num_layers + 1.
* Don't forget to free once you're done.
* @param[in]  input_length   is the length of the input array
* @param[in]  num_layers     is the number of neuron/weight layers
* in the network, i.e. the total number of layers excluding the input layer.
* (also equal to the dimensions of the weight array).
* @param[in]  weight_dims    is an array describing the shape of the weights
* array.
******************************************************************************/
int feedforward(float *input, float **weights, activation_t act, float **layers, float **pre_act_layers,
    unsigned int input_length, unsigned int num_nodes, unsigned int *weight_dims);




#endif