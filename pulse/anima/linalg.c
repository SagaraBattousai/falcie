
#include <pulse/linalg.h>

float vector_dot(float *in1, float *in2, const unsigned int dim)
{
	float out = 0;
	matrix_multiply(in1, in2, &out, 1, dim, 1);
	return out;
}
