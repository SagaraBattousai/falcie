#include <sstream>
#include <cstdlib>
#include <cstring>

#include <anima/anima-matrix.h>
#include <etro/matrix.h>

//Cpp functions to call from C using typedef
matrix_t* new_matrix(float *data, int64_t nd, intptr_t const *dims)
{

	intptr_t total_element_count = 1;

	for (int64_t i = 0; i < nd; i++)
	{
		total_element_count *= dims[i];
	}

	anima::Dimensions dimensions{ {dims, dims + nd} };

	return new anima::Matrix<float>({ data, (std::size_t) total_element_count }, dimensions);
}

const char* matrix_as_string(matrix_t *matrix, size_t *str_size)
{
	std::ostringstream oss;
	oss << *matrix << std::flush;//std::endl;
	std::string str = oss.str();
	*str_size = str.size();
	//malloc'd so it can be free'd from C
	char* ret = (char *)malloc((*str_size) * sizeof(char));
	if (ret == NULL)
	{
		return NULL;
	}

	//or &?
	std::memcpy(ret, str.c_str(), *str_size);

	return ret;
}

int64_t get_matrix_dimension_count(matrix_t *matrix)
{
	return matrix->Shape().DimensionCount();
}

//May need to change intptr_t to int64_t
intptr_t const* get_matrix_dimensions(matrix_t *matrix)
{
	return matrix->Shape().AsArray();
}

const float* get_matrix_data(matrix_t *matrix)
{
	return matrix->Data();
}

void delete_matrix(matrix_t *matrix)
{
	delete matrix;
}
