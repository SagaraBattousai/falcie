#include <stdlib.h>
#include <string.h>
#include <pulse/anima.h>
#include <pulse/linalg.h>

/*
def feedforward(inp,weights):
	prev_layer=inp;
	prev_layer=np.append(prev_layer,1);#add the bias
	curlayers=[prev_layer];
	bcurlayers=[prev_layer];#current neuron states (before activation)
	nolayers=len(weights);
	for l in range(nolayers):
		no_neurons=weights[l].shape[0]
		thislayer=np.zeros(no_neurons);
		bthislayer=np.zeros(no_neurons);#before activation
		for i in range(no_neurons):
			bthislayer[i]=np.dot(prev_layer,weights[l][i]);#(before activation)
			thislayer[i]=sigmoid(bthislayer[i]);
		prev_layer=thislayer;
		curlayers.append(np.array(thislayer));
		bcurlayers.append(np.array(bthislayer));#add to the layers (before activation)
	return curlayers,bcurlayers;
*/
int feedforward(neural_network_t *network, feedforward_tracking_t *tracking)
{
	tracking->total_layer_count = network->num_layers + 1;
	tracking->layers = (float **)malloc(sizeof(float*) * tracking->total_layer_count);
	if (tracking->layers == NULL)
	{
		return -2;
	}

	tracking->pre_activation_layers = (float **)malloc(sizeof(float*) * tracking->total_layer_count);
	if (tracking->pre_activation_layers == NULL)
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
	tracking->layers[0] = prev_layer;
	tracking->pre_activation_layers[0] = prev_layer;
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
		*(tracking->layers + i + 1) = thislayer;
		*(tracking->pre_activation_layers + i + 1) = bthislayer;
		prev_layer_size = num_neurons;
	}
	return 0;
}

/*
* def backpropagation(desired,curlayers,bcurlayers,weights,learning_rate):
	newweights=np.copy(weights);
	nolayers=len(weights);
	output=curlayers[nolayers];
	delta=aconst*(desired-output)*output*(1-output);#the output neuron
	for l in reversed(range(nolayers)):#propagate backwards
		pdelta=np.array([delta],dtype=object);#convert it to a 2D array for transpose
		newweights[l]=weights[l]+ learning_rate*curlayers[l]*pdelta.T;
		delta=aconst*curlayers[l]*(1-curlayers[l])*(np.dot(delta,weights[l]));
	return newweights;
*/
int backpropagation(float ***new_weights, float *desired, neural_network_t *network, feedforward_tracking_t *tracking)
{
	copy_weights(new_weights, network->weights, network->num_layers, network->input_size, network->neuron_dims);

	//Output neurons
	int64_t curr_neuron_count = network->neuron_dims[network->num_layers - 1];
	float *delta = (float *)malloc(sizeof(float) * curr_neuron_count);
	if (delta == NULL)
	{
		return -1;
	}
	float *output = tracking->layers[tracking->total_layer_count - 1];
	for (int64_t i = 0; i < curr_neuron_count; i++)
	{
		//MSVC doesn't like writing to the syntactical sugar [i]
		*(delta + i) = network->delta_activation(output[i]) * (desired[i] - output[i]);
	}
	///////////////////////////////////////////////////////////////////////////////

	// The neuron count for the next layer, which is actually
	// the previous layer in the network, as we are backtracking.
	// i.e. back_neurons <- curr_neurons.
	int64_t back_neuron_count;
	int64_t weight_size;

	for (int64_t j = network->num_layers - 1; j >= 0; j--)
	{
		curr_neuron_count = network->neuron_dims[j];
		if (j == 0)
		{
			back_neuron_count = network->input_size +1;
		}
		else
		{
			back_neuron_count = network->neuron_dims[j - 1];
		}
		weight_size = (back_neuron_count * curr_neuron_count);
		
		float *weightupdate = (float*)malloc(sizeof(float) * weight_size);
		if (weightupdate == NULL)
		{
			return -1;
		}

		//weightupdate = curlayers[l] * pdelta.T
		broadcast_vectors(tracking->layers[j], delta, weightupdate,
			back_neuron_count, curr_neuron_count, mult());

		//learning_rate* weight_update
		scalar_vector_mult(network->learning_rate, weightupdate, weightupdate,
			weight_size);

		vector_vector_sum(network->weights[j], weightupdate, *((*new_weights) + j),
			weight_size);

		free(weightupdate);

		if (j != 0)
		{
			float *next_delta = (float*)malloc(sizeof(float) * back_neuron_count);

			for (int64_t k = 0; k < back_neuron_count; k++)
			{
				*(next_delta + k) = network->delta_activation(*(tracking->layers[j] + k));
			}
			float *delta_weights = (float *)calloc(back_neuron_count, sizeof(float));
			matrix_multiply(network->weights[j], delta, delta_weights,
				back_neuron_count, curr_neuron_count, 1);

			free(delta);
			delta = (float *)malloc(sizeof(float) * back_neuron_count);
			if (delta == NULL)
			{
				return -1;
			}
			vector_vector_mult(next_delta, delta_weights, delta, back_neuron_count);
			free(next_delta);
			free(delta_weights);
		}
	}

	free(delta);
	return 0;
}




//Should make feedforward_tracking_t an opaque pointer
//so that freeing also frees the struct itself
//since at the moment we cant tell if it wascmalloc'd
void free_tracking(feedforward_tracking_t *tracking)
{
	for (int64_t i = 0; i < tracking; i++)
	{
		free(tracking->layers[i]);
		free(tracking->pre_activation_layers[i]);
	}

	free(tracking->layers);
	free(tracking->pre_activation_layers);
}


int copy_weights(float ***dst, float**src, int64_t num_layers, int64_t input_dim, int64_t *neuron_dims)
{
	*dst = (float**)malloc(sizeof(float*) * num_layers);
	if (*dst == NULL)
	{
		return -2;
	}
	int64_t weight_dims;// = input_dim * neuron_dims[0];
	for (int64_t i = 0; i < num_layers; i++)
	{
		if (i == 0)
		{
			weight_dims = (input_dim + 1) * neuron_dims[i];
		}
		else
		{
			weight_dims = neuron_dims[i - 1] * neuron_dims[i];
		}
		 
		*((*dst) + i) = (float *)malloc(sizeof(float) * weight_dims);
		if (*((*dst) + i) == NULL)
		{
			return -1;
		}
		memcpy(*((*dst) + i), *(src + i), (sizeof(float) * weight_dims));
	}
	return 0;
}


