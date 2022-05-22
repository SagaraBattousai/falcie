module;

#include <cstdlib>
#include <cstdint>
#include <cstring>

#ifdef USE_AVX_INTRIN
#include <immintrin.h>
#endif // USE_AVX_INTRIN


module anima:matrix;


namespace pulse
{
    namespace anima
    {


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

    } //namespace pulse::anima
} //namespace pulse
