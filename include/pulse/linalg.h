#ifndef __PULSE_ANIMA_LINALG_H__
#define __PULSE_ANIMA_LINALG_H__

#include <stdint.h>
//Maybe everything should just be in this file but for now...
#include <pulse/matrix.h>

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
void scalar_vector_sum(const float scalar, float *vector, float *out, const int64_t dim);

//TODO: AVX implemntation
void scalar_vector_mult(const float scalar, float *vector, float *out, const int64_t dim);

//TODO: AVX implemntation
void vector_vector_sum(float *vector1, float *vector2, float *out, const int64_t dim);

//TODO: AVX implemntation
void vector_vector_sub(float *vector1, float *vector2, float *out, const int64_t dim);

//TODO: AVX implemntation
void vector_vector_mult(float *vector1, float *vector2, float *out, const int64_t dim);




#endif