
extern "C" {
#include <pulse/matrix.h>
}
#include<iostream>



int main()
{

	float in1[6] = { 7,  8,  9, 10, 11, 12 };
	float in2[6] = { 27, 28, 29, 30, 31, 32 };
	float expected[6] = { 700, 724, 961, 994 };
	float actual[6] = { 0 };
	matrix_multiply(in1, in2, actual, 2, 3, 2);

	return 0;
}