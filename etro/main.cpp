/*
#define _CRTDBG_MAP_ALLOC 1

extern "C" {
//#include <pulse/internal/anima/ai.h>
//#include <pulse/internal/anima/linalg.h>
#include <pulse/anima.h>
#include <pulse/cactuar.h>
}
#include<ctime>
#include<cstring>
#include<cstdint>
#include<cmath>
#include<cfloat>
#include<cstdlib>
#include<iostream>
#include <crtdbg.h>


float sq_error(float *output, float *desired, int64_t dim)
{
	float *diff = (float *)malloc(sizeof(float) * dim);

	vector_vector_sub(output, desired, diff, dim);
	float ret = vector_dot(diff, diff, dim);
	free(diff);
	return ret;
}

float totalerr_energy(float *output, float *desired, int64_t dim)
{
	return sq_error(output, desired, dim) / 2.f;
}


float act(float x)
{
	return 1.f / (1.f + expf(-2.f * x));
}

float dact(float x)
{
	float actx = act(x);
	return 2 * actx * (1 - actx);
}

//For Testing
void fixed_weights(float ***weight_ptr, int64_t no_layers, int64_t no_inputs, int64_t *nd)
{
	*weight_ptr = (float **)malloc(sizeof(float*) * no_layers);
	if (*weight_ptr == NULL)
	{
		exit(-6);
	}
	float **weights = *weight_ptr;


	int64_t prev_dims = no_inputs + 1;
	int64_t weight_size;

	float weight_range_value = 0.001;
	float weight_range_inc = 0.05;

	for (int64_t i = 0; i < no_layers; i++)
	{
		weight_size = nd[i] * prev_dims;
		*(weights + i) = (float *)malloc(sizeof(float) * weight_size);
		if (*(weights + i) == NULL)
		{
			exit(-5);
		}
		for (int64_t j = 0; j < weight_size; j++)
		{
			*(*(weights + i) + j) = weight_range_value;
			weight_range_value += weight_range_inc;
		}
		prev_dims = nd[i];
	}
}

int main()
{

	int64_t no_inputs = 2;
	int64_t no_output = 1;

	float training_data[4][2] = { {0.f, 0.f},  {0.f, 1.f}, {1.f, 0.f}, {1.f, 1.f} };
	float desired_outputs[4] = { 0.f,       1.f,      1.f,      0.f };

	float learning_rate = 0.9;

	int max_epochs = 2;// 5000;
	float target_error = 0.0001;

	int64_t no_layers = 2;
	int64_t nd[2] = { 3, 1 };

	float **weights;

	//Not 100% safe
	//unsigned time_seed = (unsigned)time(NULL);
	//unsigned seed = 0x56324;
	//rand_weights(&weights, no_layers, no_inputs, nd, &seed);

	fixed_weights(&weights, no_layers, no_inputs, nd);


	neural_network_t net = { no_inputs,
							NULL,
							no_layers,
							nd,
							weights,
							&act,
							&dact,
							learning_rate };


	int64_t no_training_data = 4;
	float avg_error = 1.f;
	int epoch = 0;

	float **new_weights;

	feedforward_tracking_t track;

	while (avg_error > target_error && epoch < max_epochs)
	{
		//for (int64_t j = 0; j < no_training_data; j++)
		//{
		net.input = training_data[1];
		float *desired = desired_outputs + 1;

		feedforward(&net, &track);

		float *output = track.layers[no_layers];

		backpropagation(&new_weights, desired, &net, &track);

		avg_error += totalerr_energy(output, desired, 1);

		int64_t prev_dims = no_inputs + 1;
		int64_t weight_size;

		std::cout << "Weights:\n";
		for (int x = 0; x < no_layers; x++)
		{
			std::cout << "[\n";
			weight_size = prev_dims * nd[x];
			for (int y = 0; y < weight_size; y++)
			{
				if (y % prev_dims == 0)
					std::cout << "[ ";
				std::cout << net.weights[x][y] << ", ";
				if (y % prev_dims == (prev_dims - 1))
					std::cout << "]\n";
			}
			prev_dims = nd[x];
			std::cout << "]\n\n";
		}

		std::cout << std::endl << std::endl;

		std::cout << "bcurrlayers:\n";
		int64_t blayer_dims = no_inputs + 1;
		for (int x = 0; x < no_layers + 1; x++)
		{
			std::cout << "[ ";
			for (int y = 0; y < blayer_dims; y++)
			{
				std::cout << track.pre_activation_layers[x][y] << ", ";
			}
			blayer_dims = nd[x];
			std::cout << "]\n";
		}

		std::cout << std::endl << std::endl;

		std::cout << "currlayers:\n";
		int64_t layer_dims = no_inputs + 1;
		for (int x = 0; x < no_layers + 1; x++)
		{
			std::cout << "[ ";
			for (int y = 0; y < layer_dims; y++)
			{
				std::cout << track.layers[x][y] << ", ";
			}
			layer_dims = nd[x];
			std::cout << "]\n";
		}

		std::cout << std::endl << std::endl;

		std::cout << "Output: " << *output << std::endl;

		//MAnual cheat for now
		free(net.weights[0]);
		free(net.weights[1]);
		free(net.weights);

		net.weights = new_weights;
		new_weights = NULL;

		prev_dims = no_inputs + 1;
		std::cout << "New Weights:\n";
		for (int x = 0; x < no_layers; x++)
		{
			std::cout << "[\n";
			weight_size = prev_dims * nd[x];
			for (int y = 0; y < weight_size; y++)
			{
				if (y % prev_dims == 0)
					std::cout << "[ ";
				std::cout << net.weights[x][y] << ", ";
				if (y % prev_dims == (prev_dims - 1))
					std::cout << "]\n";
			}
			prev_dims = nd[x];
			std::cout << "]\n\n";
		}

		std::cout << std::endl << std::endl;

		//free_tracking(&track);
		//track = { 0 };
	//}

		avg_error /= no_training_data;
		epoch++;
	}

	std::cout << "no epochs: " << epoch << " averaged error: " << avg_error << std::endl;

	std::cout << "--------------------------------" << std::endl;
	std::cout << "Inferencing results" << std::endl;

	for (int64_t j = 0; j < no_training_data; j++)
	{
		net.input = training_data[j];
		float *desired = desired_outputs + j;

		feedforward(&net, &track);

		float *output = track.layers[no_layers];

		std::cout << "input: " << *net.input << ", " << *(net.input + 1) << " desired : "
			<< *desired << " NN output : " << *output << std::endl;
	}
	_CrtDumpMemoryLeaks();

	return 0;
}
*/

int main(void)
{
	return 0;
}