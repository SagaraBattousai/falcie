/* Can't trust __AVX__ to be defined for non MSVC.
 * Therefore setting USE_AVX_INTRIN from CMAKE if compiled with AVX.
 * 
 * Look at matrix_avx.c for intrinsics version.
 */

#ifndef USE_AVX_INTRIN

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