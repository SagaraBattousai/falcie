#ifndef __PULSE_ANIMA_MATRIX_H__
#define __PULSE_ANIMA_MATRIX_H__

#include <stdint.h>

/***************************************************************************
* @Pre **in1**'s shape is [dim1 * dim2], **in2**'s shape is [dim2 * dim3]
* and **out**'s shape is [dim1 * dim3]. Also **out** must be zero'd (for
* example via calloc or out[dim1 * dim3] = {0})
***************************************************************************/
void matrix_multiply(float *in1, float *in2, float *out,
	const int64_t dim1, const int64_t dim2, const int64_t dim3);

#endif
