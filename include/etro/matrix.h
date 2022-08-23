#ifndef __ETRO_MATRIX_H__
#define __ETRO_MATRIX_H__

#ifdef __cplusplus
#include <cstdint>
#include <anima/anima-matrix.h>
#else
#include <stdint.h>
#endif

//Typedefs for C vs CPP
#ifdef __cplusplus
typedef pulse::Matrix<float> matrix_t;
#else
typedef struct matrix matrix_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//Cpp functions to call from C using typedef
	matrix_t* new_matrix(float *data, int64_t nd, intptr_t const *dims);
	const char* matrix_as_string(matrix_t *matrix, size_t *str_size);
	int64_t get_matrix_dimension_count(matrix_t *matrix);
	intptr_t const* get_matrix_dimensions(matrix_t *matrix);
	float* get_matrix_data(matrix_t *matrix);
	void delete_matrix(matrix_t *matrix);


#ifdef __cplusplus
}
#endif



#endif