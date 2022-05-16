
#define _CRTDBG_MAP_ALLOC

#include<stdlib.h>
#include <crtdbg.h>

#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<math.h>
#include<float.h>

#include <pulse/anima.h>
#include <pulse/linalg.h>
#include <pulse/cactuar.h>


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
	//return max(x, 0.001f * x);
}

float dact(float x)
{
	float actx = act(x);
	return 2 * actx * (1 - actx);
	//return x < 0 ? 0.001f : 1;
}

void rand_weights(float ***weight_ptr, int64_t no_layers, int64_t no_inputs, int64_t *nd, unsigned *seed)
{
	frand(seed);


	*weight_ptr = (float **)malloc(sizeof(float*) * no_layers);
	if (*weight_ptr == NULL)
	{
		exit(-6);
	}
	float **weights = *weight_ptr;



	int64_t prev_dims = no_inputs + 1;
	int64_t weight_size;
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
			*(*(weights + i) + j) = frand(NULL);
		}
		prev_dims = nd[i];
	}
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

void xor_answer_weights(float ***weight_ptr)
{
	*weight_ptr = (float **)malloc(sizeof(float*) * 2);
	if (*weight_ptr == NULL)
	{
		exit(-6);
	}
	float **weights = *weight_ptr;
	
	*weights = (float *)malloc(sizeof(float) * 9);
	*((*weights) + 0) = -3.38095519250527;
	*((*weights) + 1) = 0.27972428792984166;
	*((*weights) + 2) = -2.8680051895311727;

	*((*weights) + 3) = -3.4190586203276974;
	*((*weights) + 4) = 0.14586615215925994;
	*((*weights) + 5) = -2.910842500654409;

	*((*weights) + 6) = 1.381314876529517;
	*((*weights) + 7) = 1.1241641275121095;
	*((*weights) + 8) = 4.2964259197118375;


	*(weights + 1) = (float *)malloc(sizeof(float) * 3);
	*(*(weights + 1) + 0) = -5.225825694260117;
	*(*(weights + 1) + 1) = -2.454032718939994;
	*(*(weights + 1) + 2) = 4.8864652516229805;
}

int main()
{

	int64_t no_inputs = 2;
	int64_t no_output = 1;

	float training_data[4][2] = { {0.f, 0.f},  {0.f, 1.f}, {1.f, 0.f}, {1.f, 1.f} };
	float desired_outputs[4] = { 0.f,       1.f,      1.f,      0.f };

	float learning_rate = 0.9;

	int max_epochs = 5000; //2824;//<- for fixed weights
	float target_error = 0.0001;

	int64_t no_layers = 2;
	int64_t nd[2] = { 3, 1 };

	float **weights;

	//Not 100% safe
	unsigned seed = (unsigned)time(NULL);
	//unsigned seed = 0x56324;
	rand_weights(&weights, no_layers, no_inputs, nd, &seed);

	//fixed_weights(&weights, no_layers, no_inputs, nd);


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
		for (int64_t j = 0; j < no_training_data; j++)
		{
			net.input = training_data[j];
			float *desired = desired_outputs + j;

			if (feedforward(&net, &track) != 0)
			{
				exit(-371);
			}

			float *output = track.layers[no_layers];

			if (backpropagation(&new_weights, desired, &net, &track) != 0)
			{
				exit(-372);
			}

			avg_error += totalerr_energy(output, desired, 1);

			/*
			int64_t prev_dims = no_inputs + 1;
			int64_t weight_size;
			printf("Weights:\n");
			for (int x = 0; x < no_layers; x++)
			{
				printf("[\n");
				weight_size = prev_dims * nd[x];
				for (int y = 0; y < weight_size; y++)
				{
					if (y % prev_dims == 0)
						printf("[ ");
					printf("%f, ", net.weights[x][y]);
					if (y % prev_dims == (prev_dims - 1))
						printf("]\n");
				}
				prev_dims = nd[x];
				printf("]\n\n");
			}

			printf("\n\n");

			printf("bcurrlayers:\n");
			int64_t blayer_dims = no_inputs + 1;
			for (int x = 0; x < no_layers + 1; x++)
			{
				printf("[ ");
				for (int y = 0; y < blayer_dims; y++)
				{
					printf("%f, ", track.pre_activation_layers[x][y]);
				}
				blayer_dims = nd[x];
				printf("]\n");
			}

			printf("\n\n");

			printf("currlayers:\n");
			int64_t layer_dims = no_inputs + 1;
			for (int x = 0; x < no_layers + 1; x++)
			{
				printf("[ ");
				for (int y = 0; y < layer_dims; y++)
				{
					printf("%f, ", track.layers[x][y]);
				}
				layer_dims = nd[x];
				printf("]\n");
			}

			printf("\n\n");

			printf("Output: %f\n", *output);
			*/
			//Manual cheat for now
			free(net.weights[0]);
			free(net.weights[1]);
			free(net.weights);

			net.weights = new_weights;
			new_weights = NULL;
			/*
			prev_dims = no_inputs + 1;
			printf("New Weights:\n");
			for (int x = 0; x < no_layers; x++)
			{
				printf("[\n");
				weight_size = prev_dims * nd[x];
				for (int y = 0; y < weight_size; y++)
				{
					if (y % prev_dims == 0)
						printf("[ ");
					printf("%f, ", net.weights[x][y]);
					if (y % prev_dims == (prev_dims - 1))
						printf("]\n");
				}
				prev_dims = nd[x];
				printf("]\n\n");
			}

			printf("\n\n");
			*/
			
			free_tracking(&track);

		}

		avg_error /= (float)no_training_data;
		epoch++;
	}

	//xor_answer_weights(&net.weights);


	printf("no epochs: %i averaged error: %f\n", epoch, avg_error);
	printf("##############################\n");

	printf("[[ %f, %f, %f ]\n[ %f, %f, %f ]\n %f, %f, %f ]]\n",
		net.weights[0][0],
		net.weights[0][1],
		net.weights[0][2],
		net.weights[0][3],
		net.weights[0][4],
		net.weights[0][5],
		net.weights[0][6],
		net.weights[0][7],
		net.weights[0][8]
		);

	printf("\n[[ %f, %f, %f ]]\n",
		net.weights[1][0],
		net.weights[1][1],
		net.weights[1][2]
	);


	printf("--------------------------------\n");
	printf("Inferencing results\n");

	for (int64_t j = 0; j < no_training_data; j++)
	{
		net.input = training_data[j];
		float *desired = desired_outputs + j;

		feedforward(&net, &track);

		float *output = track.layers[no_layers];

		printf("input: %f, %f desired: %f.....NN Output: %f\n", net.input[0], net.input[1], *desired, *output);

		free_tracking(&track);
	}
	//Manual cheat for now
	free(net.weights[0]);
	free(net.weights[1]);
	free(net.weights);

	_CrtDumpMemoryLeaks();

	return 0;
}