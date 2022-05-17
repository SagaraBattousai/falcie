

#define _CRTDBG_MAP_ALLOC

#include<stdlib.h>
#include <crtdbg.h>

#include<time.h>

#include <string.h>

#include <pulse/anima.h>
#include <anima/ai.h>
#include <anima/linalg.h>

#define FRAND_DIV ((float)RAND_MAX + 0.02)


neural_network_t* new_neural_network(int64_t input_size, int64_t num_layers,
	int64_t *neuron_dims, activation_t activation,
	activation_t delta_activation, float learning_rate, unsigned* seed)
{
	float **weights;
	//Technically not safe but unlikly not an issue
	unsigned weight_seed = seed != NULL? *seed : (unsigned)time(NULL);

	rand_weights(&weights, num_layers, input_size, neuron_dims, &weight_seed);

	neural_network_t *network = (neural_network_t*)malloc(sizeof(neural_network_t));
	if (network == NULL)
	{
		return NULL;
	}

	network->input_size = input_size;
	network->input = NULL;
	network->num_layers = num_layers;
	network->neuron_dims = neuron_dims;
	network->weights = weights;
	network->activation = activation;
	network->delta_activation = delta_activation;
	network->learning_rate = learning_rate;
	network->tracking = NULL;

	return network;
}

void free_neural_network(neural_network_t *network)
{
	free(network->input);
	free_tracking(network->tracking);

	for (int64_t i = 0; i < network->num_layers; i++)
	{
		free(network->weights[i]);
	}
	free(network->weights);
	free(network);
}

void set_input(neural_network_t *network, float *input)
{
	if (network->input != NULL)
	{
		free(network->input);
	}
	size_t input_byte_size = sizeof(float) * network->input_size;
	network->input = (float *)malloc(input_byte_size);

	memcpy(network->input, input, input_byte_size);
}

//TODO: Make weights opaque ptr so that we can force it to be malloced so we can 
//just change pointers and not have to copy memory (especially since 95% of the time
//weights will have been malloced
void set_weight(neural_network_t *network, float **weights)
{
	free(network->weights);
	network->weights = weights;
}

void update_weight(neural_network_t *network, float** deltaweights)
{
	int64_t prev_dimensions = network->input_size + 1;
	int64_t next_dimensions;
	int64_t weight_size;

	for (int64_t i = 0; i < network->num_layers; i++)
	{
		next_dimensions = network->neuron_dims[i];
		weight_size = prev_dimensions * next_dimensions;

		vector_vector_sum(network->weights[i], deltaweights[i], 
			network->weights[i], weight_size);

		prev_dimensions = next_dimensions;
	}

}

void rand_weights(float ***weight_ptr, int64_t no_layers, int64_t no_inputs, int64_t *nd, unsigned *seed)
{
	if (seed != NULL)
	{
		frand(seed);
	}

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

int feedforward(neural_network_t *network)
{
	if (network->tracking != NULL)
	{
		free_tracking(network->tracking);
	}

	network->tracking = (feedforward_tracking_t *)malloc(sizeof(feedforward_tracking_t));

	network->tracking->total_layer_count = network->num_layers + 1;

	network->tracking->layers = (float **)malloc(sizeof(float*) * network->tracking->total_layer_count);
	if (network->tracking->layers == NULL)
	{
		return -2;
	}

	network->tracking->pre_activation_layers = (float **)malloc(sizeof(float*) * network->tracking->total_layer_count);
	if (network->tracking->pre_activation_layers == NULL)
	{
		return -2;
	}

	int64_t prev_layer_size = (network->input_size + 1);
	float *prev_layer = (float *)malloc(sizeof(float) * prev_layer_size);
	if (prev_layer == NULL)
	{
		return -1;
	}
	*prev_layer = 1.f; //The bias term
	memcpy(prev_layer + 1, network->input, (sizeof(float) * network->input_size));

	//VV USe when using xor_answer_weights
	//memcpy(prev_layer, network->input, (sizeof(float) * network->input_size));
	//*(prev_layer + network->input_size) = 1.f; //The bias term


	network->tracking->layers[0] = prev_layer;
	network->tracking->pre_activation_layers[0] = prev_layer;
	float *thislayer;
	float *bthislayer;

	for (int64_t i = 0; i < network->num_layers; i++)
	{
		int64_t num_neurons = network->neuron_dims[i];

		thislayer = (float *)malloc(sizeof(float) * num_neurons);
		if (thislayer == NULL)
		{
			return -1;
		}

		//Must be zero'd for the matrix multiplication
		bthislayer = (float *)calloc(num_neurons, sizeof(float));
		if (bthislayer == NULL)
		{
			return -1;
		}

		//Weight Multiplication
		matrix_multiply(prev_layer, network->weights[i], bthislayer, 1, prev_layer_size, num_neurons);

		for (int64_t j = 0; j < num_neurons; j++)
		{
			//msvc doesn't like [i] access with non stack buffers
			*(thislayer + j) = network->activation(bthislayer[j]);
		}
		prev_layer = thislayer;
		*(network->tracking->layers + i + 1) = thislayer;
		*(network->tracking->pre_activation_layers + i + 1) = bthislayer;
		prev_layer_size = num_neurons;
	}
	return 0;
}

int backpropagation(float ***delta_weights, float *desired, neural_network_t *network)
{
	copy_weights(delta_weights, NULL, network->num_layers,
		network->input_size, network->neuron_dims);

	// Output neurons
	float *output = network->tracking->layers[network->num_layers];

	int64_t curr_neuron_count = network->neuron_dims[network->num_layers - 1];

	float *delta = (float *)malloc(sizeof(float) * curr_neuron_count);
	if (delta == NULL)
	{
		return -1;
	}

	for (int64_t i = 0; i < curr_neuron_count; i++)
	{
		//*(delta + i) = network->delta_activation(output[i]) * (desired[i] - output[i]);
		*(delta + i) = 2 * output[i] * (1 - output[i]) * (desired[i] - output[i]);
	}

	// The neuron count for the next layer, which is actually
	// the previous layer in the network, as we are backtracking.
	// i.e. back_neurons <- curr_neurons.
	int64_t back_neuron_count;
	int64_t weight_size;

	for (int64_t j = network->num_layers - 1; j >= 0; j--)
	{
		curr_neuron_count = network->neuron_dims[j];

		back_neuron_count = j == 0 ? network->input_size + 1 : network->neuron_dims[j - 1];
		
		weight_size = (back_neuron_count * curr_neuron_count);

		//weightupdate = curlayers[l] * pdelta.T
		broadcast_vectors(network->tracking->layers[j], delta, *(*delta_weights + j),
			back_neuron_count, curr_neuron_count, mult());

		//learning_rate* weight_update
		scalar_vector_mult(network->learning_rate, *(*delta_weights + j),
			*(*delta_weights + j), weight_size);

		if (j != 0)
		{
			float *next_delta = (float*)malloc(sizeof(float) * back_neuron_count);
			
			//delta=aconst*curlayers[l]*(1-curlayers[l])*(np.dot(delta,weights[l]));

			for (int64_t k = 0; k < back_neuron_count; k++)
			{
				float clayer = *(network->tracking->layers[j] + k);
				//*(next_delta + k) = network->delta_activation(*(tracking->layers[j] + k));
				*(next_delta + k) = 2 * clayer * (1 - clayer);
			}
			float *delta_update = (float *)calloc(back_neuron_count, sizeof(float));

			matrix_multiply(network->weights[j], delta, delta_update,
				back_neuron_count, curr_neuron_count, 1);

			free(delta);
			delta = (float *)malloc(sizeof(float) * back_neuron_count);
			if (delta == NULL)
			{
				return -1;
			}
			vector_vector_mult(next_delta, delta_update, delta, back_neuron_count);
			
			free(next_delta);
			free(delta_update);
		}
	}

	free(delta);
	return 0;
}


void get_network_output(neural_network_t *network, float **output)
{
	if (network->tracking == NULL)
	{
		feedforward(network);
	}

	int64_t output_size = network->neuron_dims[network->num_layers - 1];

	*output = (float*)malloc(sizeof(float) * output_size);
	if (*output == NULL)
	{
		return;
	}

	//network->num_layers is always 1 less than tracking->total_layer_count
	memcpy(*output, network->tracking->layers[network->num_layers], output_size * sizeof(float));
}


//Should make feedforward_tracking_t an opaque pointer
//so that freeing also frees the struct itself
//since at the moment we cant tell if it wascmalloc'd
void free_tracking(feedforward_tracking_t *tracking)
{
	for (int64_t i = 0; i < tracking->total_layer_count; i++)
	{

		free(tracking->layers[i]);
		if (i > 0)
		{
			free(tracking->pre_activation_layers[i]);
		}
	}

	free(tracking->layers);
	free(tracking->pre_activation_layers);
	free(tracking);
}

int copy_weights(float ***dst, float**src, int64_t num_layers, int64_t input_size, int64_t *neuron_dims)
{
	*dst = (float**)malloc(sizeof(float*) * num_layers);

	if (*dst == NULL)
	{
		return -2;
	}

	int64_t prev_dimensions = input_size + 1;
	int64_t next_dimensions;
	int64_t weight_size;

	for (int64_t i = 0; i < num_layers; i++)
	{
		next_dimensions =  neuron_dims[i];
		weight_size = prev_dimensions * next_dimensions;
		 
		*(*dst + i) = (float *)malloc(sizeof(float) * weight_size);
		
		if (*(*dst + i) == NULL)
		{
			return -1;
		}
		if (src != NULL)
		{
			memcpy(*(*dst + i), *(src + i), (sizeof(float) * weight_size));
		}
	}
	return 0;
}

float frand(unsigned int *seed)
{
	if (seed != NULL)
	{
		srand(*seed);
	}

	return rand() / FRAND_DIV;
}

