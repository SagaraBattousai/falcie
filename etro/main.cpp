
extern "C" {
#include <pulse/anima.h>
}
#include<cstring>
#include<cstdint>
#include<cmath>
#include<cstdlib>
#include<iostream>

float act(float x)
{
	return 1.f / (1.f + expf( -2.f * x));
}

float dact(float x)
{
	float actx = act(x);
	return 2 * actx * (1 - actx);
}

int main()
{
	float input[2] = { 0, 1 };

	//float l1w[9] = { 1, 2, 3, 
	//				 4, 5, 6, 
	//				 7, 8, 9 };

	float l1w[9] = { 1, 4, 7,
					 2, 5, 8,
					 3, 6, 9 };


	float l2w[3] = { 10, 11, 12 };

	float* weights[2] = { l1w, l2w };
	
	int64_t nd[2] = { 3, 1 };

	neural_network_t net = {2,
							input,
							2,
							nd,
							weights,
							&act,
							&dact,
							0.9};

	//float **layers = (float **)malloc(sizeof(float*) * 3);
	//float **blayers = (float **)malloc(sizeof(float*) * 3);
	feedforward_tracking_t track;

	
	//feedforward(input, weights, &id, layers, blayers, 1, 2, wd);
	feedforward(&net, &track);
	std::cout << "[" << std::endl;
	std::cout << track.pre_activation_layers[0][0] << std::endl;
	std::cout << track.pre_activation_layers[0][1] << std::endl;
	std::cout << track.pre_activation_layers[0][2] << std::endl;

	std::cout << track.pre_activation_layers[1][0] << std::endl;
	std::cout << track.pre_activation_layers[1][1] << std::endl;
	std::cout << track.pre_activation_layers[1][2] << std::endl;

	std::cout << track.pre_activation_layers[2][0] << std::endl;
	std::cout << "]" << std::endl;


	std::cout << std::endl << std::endl;

	std::cout << "[" << std::endl;

	std::cout << track.layers[0][0] << std::endl;
	std::cout << track.layers[0][1] << std::endl;
	std::cout << track.layers[0][2] << std::endl;

	std::cout << track.layers[1][0] << std::endl;
	std::cout << track.layers[1][1] << std::endl;
	std::cout << track.layers[1][2] << std::endl;

	std::cout << track.layers[2][0] << std::endl;
	std::cout << "]" << std::endl;

	float desired = 1.f;

	float **new_weights;

	backpropagation(&new_weights, &desired, &net, &track);

	std::cout << std::endl << "***************" << std::endl;

	std::cout << "[" << std::endl;
	std::cout << new_weights[0][0] << std::endl;
	std::cout << new_weights[0][1] << std::endl;
	std::cout << new_weights[0][2] << std::endl;
	std::cout << new_weights[0][3] << std::endl;
	std::cout << new_weights[0][4] << std::endl;
	std::cout << new_weights[0][5] << std::endl;
	std::cout << new_weights[0][6] << std::endl;
	std::cout << new_weights[0][7] << std::endl;
	std::cout << new_weights[0][8] << std::endl;
	std::cout << "]" << std::endl;

	std::cout << std::endl;

	std::cout << "[" << std::endl;
	std::cout << new_weights[1][0] << std::endl;
	std::cout << new_weights[1][1] << std::endl;
	std::cout << new_weights[1][2] << std::endl;
	std::cout << "]" << std::endl;

	free(new_weights[1]);
	free(new_weights[0]);
	free(new_weights);

	return 0;
}