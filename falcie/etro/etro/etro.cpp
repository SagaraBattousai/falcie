
#include <iostream>

#include <etro/matrix.h>

int main(void)
{
	float arr1[] = { 1,2,3,4,5,6,7,8,9 };
	float arr2[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 };
	
	intptr_t dims1[] = { 3,3 };
	intptr_t dims2[] = { 3,3,1 };
	intptr_t dims3[] = { 3,2,3 };
	
	matrix_t *matrix1 = new_matrix(arr1, 2, dims1);
	matrix_t *matrix2 = new_matrix(arr1, 3, dims2);
	matrix_t *matrix3 = new_matrix(arr2, 3, dims3);

	std::cout << *matrix1;
	std::cout << *matrix2;
	std::cout << *matrix3;

	return 0;
}