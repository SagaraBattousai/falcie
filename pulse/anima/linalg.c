
#include <pulse/linalg.h>

//TODO: AVX implemntation
float vector_dot(float *in1, float *in2, const int64_t dim)
{
	float out = 0;
	matrix_multiply(in1, in2, &out, 1, dim, 1);
	return out;
}

void scalar_vector_sum(const float scalar, float *vector, float *out, const int64_t dim)
{
    for (int64_t i = 0; i < dim; i++)
    {
        *(out + i) = scalar + *(vector + i);
    }
}

//TODO: AVX implemntation
void scalar_vector_mult(const float scalar, float *vector, float *out, const int64_t dim)
{
    for (int64_t i = 0; i < dim; i++)
    {
        *(out + i) = scalar * *(vector + i);
    }
}

//TODO: AVX implemntation
void vector_vector_sum(float *vector1, float *vector2, float *out, const int64_t dim)
{
    for (int64_t i = 0; i < dim; i++)
    {
        out[i] = vector1[i] + vector2[i];
    }
}

//TODO: AVX implemntation
void vector_vector_sub(float *vector1, float *vector2, float *out, const int64_t dim)
{
    for (int64_t i = 0; i < dim; i++)
    {
        out[i] = vector1[i] - vector2[i];
    }
}


//TODO: AVX implemntation
void vector_vector_mult(float *vector1, float *vector2, float *out, const int64_t dim)
{
    for (int64_t i = 0; i < dim; i++)
    {
        out[i] = vector1[i] * vector2[i];
    }
}

//TODO: AVX implemntation
//in1 is [1 * m] and  in2 is [n * 1]. out is [n * m]
//Can't be mutable as in1 nd in2 are reused (double looped) therefore
//overwriting would change values.
void broadcast_vectors(const float *in1, const float *in2, float *out,
    const int64_t dim1, const int64_t dim2, combinator_ptr combinator)
{
    /*
    for (int64_t i = 0; i < dim2; i++)
    {
        for (int64_t j = 0; j < dim1; j++)
        {
            out[i * dim1 + j] = combinator(in1[j], in2[i]);
        }

    }
    */
    //Try flipping YES!!! Flipping worked
    //REMEMBER IM doing everything transposed to benny and numpy
    for (int64_t i = 0; i < dim1; i++)
    {
        for (int64_t j = 0; j < dim2; j++)
        {
            out[i * dim2 + j] = combinator(in1[i], in2[j]);
        }

    }
}

//Can it be static? It can, and must thanks to federated_blockchain
//also having an add function ...
static float add(float a, float b)
{
    return a + b;
}

static float multiply(float a, float b)
{
    return a * b;
}

combinator_ptr adder()
{
    return &add;
}
combinator_ptr mult()
{
    return &multiply;
}

