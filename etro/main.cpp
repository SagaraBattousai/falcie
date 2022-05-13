
extern "C" {
#include <pulse/anima.h>
}
#include <cstdlib>
#include<iostream>

float id(float x)
{
	return x;
}

int main()
{
	//float input[2] = { 0.25, 0.45 };
	float input[1] = { 0.45 };
	float* weights[2] = {0};
	float l1w[4] = {0.75, -0.34, 0.89, -0.23};
	float l2w[2] = {0.125, 0.625};
	weights[0] = l1w;
	weights[1] = l2w;

	float **layers = (float **)malloc(sizeof(float*) * 3);
	float **blayers = (float **)malloc(sizeof(float*) * 3);


	unsigned int wd[2] = {2, 1};

	feedforward(input, weights, &id, layers, blayers, 1, 2, wd);

	std::cout << layers[0][0] << std::endl;
	std::cout << layers[0][1] << std::endl;
	std::cout << layers[0][2] << std::endl;

	std::cout << layers[1][0] << std::endl;
	std::cout << layers[1][1] << std::endl;

	std::cout << layers[2][0] << std::endl;


	return 0;
}