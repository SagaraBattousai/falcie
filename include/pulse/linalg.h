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
// Similar to numpy's but only on vectors (shape output based on numpy).
//in1 is [1 * dim1] and  in2 is [dim2 * 1]. out is [dim2 * dim1]
//Note: Probably neater to take pointer to out array to show its a pointer to an array?
//But unnessicary for in1/2 so ......
void broadcast_vectors(float *in1, float *in2, float *out,
	const int64_t dim1, const int64_t dim2, combinator_ptr combinator);

//TODO: AVX implemntation
void scalar_vector_sum(const float scalar, float *vector, float *out, const int64_t dim);

//TODO: AVX implemntation
void scalar_vector_mult(const float scalar, float *vector, float *out, const int64_t dim);

//TODO: AVX implemntation
void vector_vector_sum(float *vector1, float *vector2, float *out, const int64_t dim);


//TODO: AVX implemntation
void vector_vector_mult(float *vector1, float *vector2, float *out, const int64_t dim);




#endif