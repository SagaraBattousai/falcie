
//export module anima;

//export import :matrix;
//export import :neural_network;
//export import :random;

#ifndef __PULSE_ANIMA__
#define __PULSE_ANIMA__

#include <anima/anima-matrix.h>
#include <anima/anima-neural_network.h>
#include <anima/anima-random.h>

#endif




/*


//TODO: Clean this up!!

//TODO: AVX implemntation
void vector_vector_sum(float *vector1, float *vector2, float *out, const int64_t dim);

//TODO: AVX implemntation
void vector_vector_sub(float *vector1, float *vector2, float *out, const int64_t dim);

//TODO: AVX implemntation
void scalar_vector_sum(const float scalar, float *vector, float *out, const int64_t dim);

//TODO: AVX implemntation
void scalar_vector_mult(const float scalar, float *vector, float *out, const int64_t dim);


float vector_dot(float *in1, float *in2, const int64_t dim);


void rand_weights(float ***weight_ptr, int64_t no_layers, int64_t no_inputs, int64_t *nd, unsigned *seed);

//TODO: Hide this later
int copy_weights(float ***dst, float **src, int64_t NumberOfLayers, int64_t input_dim, int64_t *neuron_dims);

//TODO: Make this better!
float frand(unsigned int *seed);



#ifndef __PULSE_ANIMA_LINALG_H__
#define __PULSE_ANIMA_LINALG_H__

#include <stdint.h>
#include<pulse/anima.h>
//Maybe everything should just be in this file but for now...
#include <anima/matrix.h>

typedef float (*combinator_ptr)(float, float);

combinator_ptr adder();
combinator_ptr mult();

//TODO: AVX implemntation
float vector_dot(float *in1, float *in2, const int64_t dim);

//TODO: AVX implemntation
// Big mistake to try and copy numpy!! Their order is wrong for you!!!
//in1 is [m * 1] and  in2 is [1 * n]. out is [m * n]
void broadcast_vectors(const float *in1, const float *in2, float *out,
    const int64_t dim1, const int64_t dim2, combinator_ptr combinator);

//TODO: AVX implemntation
void vector_vector_mult(float *vector1, float *vector2, float *out, const int64_t dim);

*/

/***************************************************************************
* @Pre **in1**'s shape is [dim1 * dim2], **in2**'s shape is [dim2 * dim3]
* and **out**'s shape is [dim1 * dim3]. Also **out** must be zero'd (for
* example via calloc or out[dim1 * dim3] = {0})
***************************************************************************/
//void matrix_multiply(float *in1, float *in2, float *out,
  //  const int64_t dim1, const int64_t dim2, const int64_t dim3);





