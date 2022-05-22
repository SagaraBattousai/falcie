module;

#include <cstdint>

#ifdef USE_AVX_INTRIN
#include <immintrin.h>
#endif // USE_AVX_INTRIN


export module anima:matrix;

import <array>;
import <vector>;


export namespace pulse
{

    template <typename T, int64_t Dim1, int64_t Dim2>
    class Matrix
    {
    public:
        Matrix(std::array<std::array<T, Dim2>, Dim1> values);
        Matrix& operator*=(const X& rhs);

        // passing lhs by value helps optimize chained calls (think about object
        // construction count. (define interms of *=).
        friend Matrix operator*(Matrix lhs, const Matrix& rhs);

    private:
        std::array<std::array<T, Dim2>, Dim1> data;
    };

    template <typename T, int64_t Dim1>
    using Vector = Matrix<T, 1, Dim1>;


    /* Can't trust __AVX__ to be defined for non MSVC.
     * Therefore setting USE_AVX_INTRIN from CMAKE if compiled with AVX.
     */

    #ifdef USE_AVX_INTRIN

    constexpr int AVX_512_PACK_SIZE = 16;
    constexpr int AVX_512_PACK_SHIFT = 4; //Used for multipying by 16

    //Overflow if not mult of 16
    static inline void inner_intrin_matrix_mult(__m512 sv, float *rowB, float *rowC)
    {
        //Can I assume aligned? i.e. do I need to use the u versions (loadu storeu etc)
        __m512 cv = _mm512_load_ps(rowC);
        __m512 bv = _mm512_load_ps(rowB);
        cv = _mm512_fmadd_ps(bv, sv, cv);
        _mm512_store_ps(rowC, cv);
    }

    void matrix_multiply(float *in1, float *in2, float *out,
        const int64_t dim1, const int64_t dim2, const int64_t dim3)
    {
        const int64_t packed_width = dim3 >> AVX_512_PACK_SHIFT; //i.e. /16

        for (int64_t i = 0; i < dim1; i++)
        {
            int64_t out_row_start = i * dim3;

            for (int64_t j = 0; j < dim2; j++)
            {
                __m512 sv = _mm512_set1_ps(in1[i * dim2 + j]); //A_ij
                int64_t in_row_start = j * dim3;

                //Can only work on 16 elements per row at a time
                for (int64_t packed_segment = 0; packed_segment < packed_width; packed_segment++)
                {
                    int64_t packed_offset = packed_segment << AVX_512_PACK_SHIFT; //i.e. * 16

                    inner_intrin_matrix_mult(
                        sv,
                        &in2[in_row_start + packed_offset],
                        &out[out_row_start + packed_offset]
                    );
                }

                // Handle remaining coloumns when the number of elements is not a multiple of
                // The packing size. In this case 16.
                int64_t columns_remaining = dim3 % AVX_512_PACK_SIZE;
                if (columns_remaining != 0)
                {
                    int64_t remaining_offset = dim3 - columns_remaining;
                    // Don't want the possibility of reading from bad location so we
                    // pad the input row too.
                    float in_padded_row[AVX_512_PACK_SIZE] = { 0 };
                    float out_padded_row[AVX_512_PACK_SIZE] = { 0 };

                    memcpy(in_padded_row, &in2[in_row_start + remaining_offset],
                        sizeof(float) * columns_remaining);
                    memcpy(out_padded_row, &out[out_row_start + remaining_offset],
                        sizeof(float) * columns_remaining);

                    inner_intrin_matrix_mult(sv, in_padded_row, out_padded_row);

                    memcpy(&out[out_row_start + remaining_offset], out_padded_row,
                        sizeof(float) * columns_remaining);
                }
            }
        }
}

#endif

#else
    #include <stdlib.h>
    #include <anima/matrix.h>
    // Don't swap j and k. That benifit removed stride from inner loop but
    // the way we chose our indicies does this automatically.
    // i.e. Inner loop var (in this case k) is not multiplied in said loop.
    void matrix_multiply(float *in1, float *in2, float *out,
        const int64_t dim1, const int64_t dim2, const int64_t dim3)
    {
      for (int64_t i = 0; i < dim1; i++)
      {
        for (int64_t j = 0; j < dim2; j++)
        {
            for (int64_t k = 0; k < dim3; k++)
            {
                out[i * dim3 + k] += in1[i * dim2 + j] * in2[j * dim3 + k];
            }
        }
      }
    }


    #endif
   
    





    #include <anima/linalg.h>

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
    //in1 is [m * 1] and  in2 is [1 * n]. out is [m * n]
    //Can't be mutable as in1 nd in2 are reused (double looped) therefore
    //overwriting would change values.
    void broadcast_vectors(const float *in1, const float *in2, float *out,
        const int64_t dim1, const int64_t dim2, combinator_ptr combinator)
    {
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


    */
} //namespace pulse
