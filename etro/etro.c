#include <stdio.h>
#include <stdlib.h>
#include <anima/anima-matrix.h>
#include <stdint.h>


int main()
{

	int *a = malloc(4 * 3);
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;

	matrix_t *matrix = new_column_matrix(a, 3);

	printf("%i, %i, %i\n",
		*((int *)matrix_get_flat(matrix, 0, 4)),
		*((int *)matrix_get_flat(matrix, 1, 4)),
		*((int *)matrix_get_flat(matrix, 2, 4)));


	return 0;
}