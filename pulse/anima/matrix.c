#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

#include <pulse/matrix.h>

void f(float *x)
{
    float y[2] = { 1.0, 2.0 };
    *(x + 2) = 2.7;
    printf("%f\n%f\n\n", *(x + 2), *(y + 1));
}
/*
void inner(int s, int *rowB, int *rowC)
{
  __m512i sv = _mm512_set1_epi32(s);
  for (int j = 0; j < ARR_SHAPE/16; j++)
  {
    //Do I need/should u? Maybe if last load isn't full?
    __m512i cv = _mm512_loadu_epi32(&rowC[16*j]); 
    __m512i bv = _mm512_loadu_epi32(&rowB[16*j]); 
    //Apparently can't do FMA from intrinsic cv = _mm512_fmadd_pd(sv, bv, cv);
    //Compiler should hopefully do it for me
    __m512i mult = _mm512_mullo_epi32(bv, sv);
    cv = _mm512_add_epi32(mult, cv);
    _mm512_storeu_epi32(&rowC[16*j], cv);   
  }
}

int *gemm3(int *matrix1, int *matrix2)
{
  int *out = calloc(ARR_SIZE, sizeof(int));
  for (int i = 0; i < ARR_SHAPE; i++)
  {
    for (int k = 0; k < ARR_SHAPE; k++)
    {
      inner(matrix1[i * ARR_SHAPE + k],
          &matrix2[k * ARR_SHAPE],
          &out[i * ARR_SHAPE]);
      }
    }
  return out;
}
*/
