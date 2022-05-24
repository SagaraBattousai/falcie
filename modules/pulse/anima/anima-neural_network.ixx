module;

#include <cstdint>
#include <algorithm>
#include <random>
//#include <iostream>

export module anima:neural_network;

import <array>;
import <vector>;

import :random;

namespace pulse
{
	namespace anima
	{
		//May change to weight ptr, I've forgotten all my stl pointer rules
		template <std::int64_t NumberOfLayers>
		using Weights = std::array<std::vector<float>, NumberOfLayers>;

		/** @c total_layer_count: The number of layers (including the
		 * input layer). Used primarily to help with freeing memory.
		 */
		template <std::int64_t LayerCount>
		struct FeedForwardTracking
		{
			/** @c layers: A matrix of the layers through the network. */
			Weights<LayerCount> layers;
			/**
			 * @c pre_act_layers: A matrix of the layers, before the activation
			 * is applied, through the network.
			 */
			Weights<LayerCount> pre_activation_layers;
		};

	} //namespace pulse::anima
}

//TODO: decide on how to handle weights (array of vectors or array of matrix?)
// array of matrix is more natural but array of vectors is better for code
// but ive moved from C to c++ now sooo....
// NOTE: Dont kill all C code as that is still needed for wrapper.
//Public api
export namespace pulse
{
	using ActivationFunction = float (*)(const float&);

	using ErrorEnergy = float (*)(float *, float *, std::int64_t);

	//May change to weight ptr, I've forgotten all my stl pointer rules
	template <std::int64_t NumberOfLayers>
	using Weights = std::array<std::vector<float>, NumberOfLayers>;

	template <std::int64_t Size>
	using Data = std::array<float, Size>;

	float sigmoid(const float&);

	float delta_sigmoid(const float&);


	/** @c input_length: The length of the input array.
	 * @c num_layers: The number of neuron / weight layers in the network,
	 * i.e. the total number of layers excluding the input layer (also equal
	 * to the dimensions of the weight array).
	 */
	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	class NeuralNetwork
	{
	public:
		NeuralNetwork(std::array<std::int64_t, NumberOfLayers> neuron_dims);// , unsigned* seed);

		NeuralNetwork(std::array<std::int64_t, NumberOfLayers> neuron_dims,
			float learning_rate);// , unsigned* seed);

		NeuralNetwork(std::array<std::int64_t, NumberOfLayers> neuron_dims,
			ActivationFunction activation, ActivationFunction delta_activation,
			float learning_rate);// , unsigned* seed);

		void SetInput(Data<InputSize> input);

		//TODO: workout true ret types
		const Weights<NumberOfLayers>& GetWeights();

		void SetWeights(Weights<NumberOfLayers> weights);

		void UpdateWeights(Weights<NumberOfLayers> delta_weights);

		Data<OutputSize> Output();

		/*****************************************************************************
		* Neural Network feed forward.
		*
		* @param[in] network The network to perform the forward pass on.
		* @param[out] tracking Records the layers and and pre activation layers
		* through a forward pass of the network. This can then be used to calculate
		* the gradients via backpropagation.
		******************************************************************************/
		void FeedForward();

		//Although it seems like a waste now, i think extracting delta weights was infact a good idea
		// Hopefully it will allow for ofloading back prop training too.
		//Maybe make **weights a struct. then just ptr to that struct.
		Weights<NumberOfLayers> Backpropagation(Data<OutputSize> desired);


		//These three are just convieience (at least the last two are) so maybe make non-member-non friend?
		// batch has size batch size and batch[x] has input size size
		void TrainNetwork(Data<OutputSize> desired, Data<InputSize>, std::int64_t epochs);

		//for now use multy ret by reference but i think well wanna change this //TODO:
		// batch has size batch size and batch[x] has input size size
		void train_network_with_stats(Data<OutputSize> desired, Data<InputSize>, std::int64_t epochs,
			std::int64_t *epochs_taken, float *avg_err, float target_err, ErrorEnergy err_func);
		/*
		void print_inferencing_results(neural_network_t *network,
			float **training_data, std::int64_t training_data_size, std::int64_t training_data_dimension,
			float **desired, std::int64_t desired_dimension);*/

		static constexpr float DEFAULT_LEARNING_RATE = 0.9;

	private:

		//TODO: Allow custom random generator and custom preset weights
		void GenerateWeights();		

		/** @c input: A pointer to the networks inputs. */
		Data<InputSize> input;

		/**
		 * @c neuron_dims is an array describing the shape
		 * of the neurons in each layer. The array must be of length @c NumberOfLayers
		 * and structured such that @c neuron_dims[layer - 1] is the number of neurons
		 * at layer @c layer
		 */
		std::array<std::int64_t, NumberOfLayers> neuron_dims;

		/**
		* @c weights: An array of weights with @c NumberOfLayers dimensions each of
		* which are described by @c neuron_dims. The true dimensions of weights,
		* since it is a 2D array,
		* is @c [neuron_dims[layer1 - 1] * neuron_dims[layer2 -1]], such that
		* the weight connects layer1 to layer2
		*/
		Weights<NumberOfLayers> weights;

		//TODO: make into an activation array.
		/**
		* @c activation: A function pointer to the
		* activation function for the network.
		*/
		ActivationFunction activation;

		ActivationFunction delta_activation;

		/** @c learning_rate: The learning rate of the network. */
		float learning_rate;

		//feedforward_tracking_t *tracking;
		anima::FeedForwardTracking<NumberOfLayers + 1> tracking;


	};

	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::NeuralNetwork(
		std::array<std::int64_t, NumberOfLayers> neuron_dims)
		: NeuralNetwork(neuron_dims, DEFAULT_LEARNING_RATE)
	{}


	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::NeuralNetwork(
		std::array<std::int64_t, NumberOfLayers> neuron_dims, float learning_rate)
		: NeuralNetwork(neuron_dims, &sigmoid, &delta_sigmoid, learning_rate)
	{}


	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::NeuralNetwork(
		std::array<std::int64_t, NumberOfLayers> neuron_dims,
		ActivationFunction activation, ActivationFunction delta_activation,
		float learning_rate) //, unsigned* seed)
		: neuron_dims(neuron_dims)
		, activation(activation)
		, delta_activation(delta_activation)
		, learning_rate(learning_rate)
		, tracking()
	{
		//TODO: clean this
		//this->weights = 
		GenerateWeights();
	}

	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	void NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::SetInput(Data<InputSize> input)
	{
		this->input = input;
	}

	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	const Weights<NumberOfLayers>& NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::GetWeights()
	{
		return this->weights;
	}

	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	void NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::SetWeights(
		Weights<NumberOfLayers> weights)
	{
		this->weights = weights;
	}

	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	void NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::UpdateWeights(
		Weights<NumberOfLayers> delta_weights)
	{
		//TODO: Vector sums
	}

	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	Data<OutputSize> NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::Output()
	{
		Data<OutputSize> output{}; //Do I need to initialize? Doesn't hurt me thinks. TODO: Try it!
		std::copy_n(this->tracking.layers.begin(), OutputSize, output);
		return output;
	}


	template <std::int64_t InputSize, std::int64_t OutputSize, std::int64_t NumberOfLayers>
	void NeuralNetwork<InputSize, OutputSize, NumberOfLayers>::GenerateWeights()
	{
		//	using Weights = std::array<std::vector<float>, NumberOfLayers>;

		//Weights initWeights{};

		//TODO: Change
		std::mt19937 bitgen{ 0x43616c6f };

		logistic_distribution<float> randGen{};

		std::int64_t largest = *std::max_element(this->neuron_dims.begin(), this->neuron_dims.end());
		std::vector<float> weightLayer{};
		weightLayer.reserve(largest);

		std::int64_t i = 0;
		for (auto it = this->neuron_dims.begin(); it != this->neuron_dims.end(); it++)
		{
			for (std::int64_t i = 0; i < *it; i++)
			{
				weightLayer.push_back(randGen(bitgen));
			}
			this->weights[i] = weightLayer;
			i++;
			weightLayer.clear();
		}

	}









} //namespace pulse


/*
		void SetInput(Data<InputSize> input);

		//TODO: workout true ret types
		const Weights<NumberOfLayers>& GetWeights();

		void SetWeights(Weights<NumberOfLayers> weights);

		void UpdateWeights(Weights<NumberOfLayers> delta_weights);

		Data<OutputSize> Output();
		*/
		/*****************************************************************************
		* Neural Network feed forward.
		*
		* @param[in] network The network to perform the forward pass on.
		* @param[out] tracking Records the layers and and pre activation layers
		* through a forward pass of the network. This can then be used to calculate
		* the gradients via backpropagation.
		******************************************************************************/
		//void FeedForward();
/*
		//Although it seems like a waste now, i think extracting delta weights was infact a good idea
		// Hopefully it will allow for ofloading back prop training too.
		//Maybe make **weights a struct. then just ptr to that struct.
		Weights<NumberOfLayers> Backpropagation(Data<OutputSize> desired);


		//These three are just convieience (at least the last two are) so maybe make non-member-non friend?
		// batch has size batch size and batch[x] has input size size
		void TrainNetwork(Data<OutputSize> desired, Data<InputSize>, std::std::int64_t epochs);

		//for now use multy ret by reference but i think well wanna change this //TODO:
		// batch has size batch size and batch[x] has input size size
		void train_network_with_stats(Data<OutputSize> desired, Data<InputSize>, std::std::int64_t epochs,
			std::std::int64_t *epochs_taken, float *avg_err, float target_err, ErrorEnergy err_func);
		*//*
		void print_inferencing_results(neural_network_t *network,
			float **training_data, std::std::int64_t training_data_size, std::std::int64_t training_data_dimension,
			float **desired, std::std::int64_t desired_dimension);*/
			/*

				};


				namespace anima
				{

				} //namespace pulse::anima
			} //namespace pulse

			*/

			/*
			neural_network_t* new_neural_network(std::std::int64_t input_size, std::std::int64_t num_layers,
				std::std::int64_t *neuron_dims, activation_t activation,
				activation_t delta_activation, float learning_rate, unsigned* seed)
			{
				float **weights;
				//Technically not safe but unlikly not an issue
				unsigned weight_seed = seed != NULL ? *seed : (unsigned)time(NULL);

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

				for (std::std::int64_t i = 0; i < network->num_layers; i++)
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

			//Todo: Change this, sharing (in C) can be dangerous;
			void get_weight(neural_network_t *network, float ***weights)
			{
				*weights = network->weights;
			}

			//TODO: Make weights opaque ptr so that we can force it to be malloced so we can  //<< ^^Bad for above
			//just change pointers and not have to copy memory (especially since 95% of the time
			//weights will have been malloced
			void set_weight(neural_network_t *network, float **weights)
			{
				free(network->weights);
				//network->weights = weights;
				copy_weights(&network->weights, weights, network->num_layers, network->input_size, network->neuron_dims);
			}

			void update_weight(neural_network_t *network, float** deltaweights)
			{
				std::int64_t prev_dimensions = network->input_size + 1;
				std::int64_t next_dimensions;
				std::int64_t weight_size;

				for (std::int64_t i = 0; i < network->num_layers; i++)
				{
					next_dimensions = network->neuron_dims[i];
					weight_size = prev_dimensions * next_dimensions;

					vector_vector_sum(network->weights[i], deltaweights[i],
						network->weights[i], weight_size);

					prev_dimensions = next_dimensions;
				}

			}

			void rand_weights(float ***weight_ptr, std::int64_t no_layers, std::int64_t no_inputs, std::int64_t *nd, unsigned *seed)
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

				std::int64_t prev_dims = no_inputs + 1;
				std::int64_t weight_size;
				for (std::int64_t i = 0; i < no_layers; i++)
				{
					weight_size = nd[i] * prev_dims;
					*(weights + i) = (float *)malloc(sizeof(float) * weight_size);
					if (*(weights + i) == NULL)
					{
						exit(-5);
					}

					for (std::int64_t j = 0; j < weight_size; j++)
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

				std::int64_t prev_layer_size = (network->input_size + 1);
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

				for (std::int64_t i = 0; i < network->num_layers; i++)
				{
					std::int64_t num_neurons = network->neuron_dims[i];

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

					for (std::int64_t j = 0; j < num_neurons; j++)
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

				std::int64_t curr_neuron_count = network->neuron_dims[network->num_layers - 1];

				float *delta = (float *)malloc(sizeof(float) * curr_neuron_count);
				if (delta == NULL)
				{
					return -1;
				}

				for (std::int64_t i = 0; i < curr_neuron_count; i++)
				{
					//*(delta + i) = network->delta_activation(output[i]) * (desired[i] - output[i]);
					*(delta + i) = 2 * output[i] * (1 - output[i]) * (desired[i] - output[i]);
				}

				// The neuron count for the next layer, which is actually
				// the previous layer in the network, as we are backtracking.
				// i.e. back_neurons <- curr_neurons.
				std::int64_t back_neuron_count;
				std::int64_t weight_size;

				for (std::int64_t j = network->num_layers - 1; j >= 0; j--)
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

						for (std::int64_t k = 0; k < back_neuron_count; k++)
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

			int train_network(float **desired, float **batch, std::int64_t batch_size,
				std::int64_t epochs, neural_network_t *network)
			{
				float **delta_weights = NULL;

				for (int i = 0; i < epochs; i++)
				{
					for (std::int64_t j = 0; j < batch_size; j++)
					{
						set_input(network, batch[j]);
						float *desired_output = desired[j];

						if (feedforward(network) != 0)
						{
							return (-371);
						}

						//get_network_output(net, &output);

						if (backpropagation(&delta_weights, desired_output, network) != 0)
						{
							return (-372);
						}

						//avg_error += totalerr_energy(output, desired, 1);

						update_weight(network, delta_weights);

						//TODO Clean this up!
						for (std::int64_t k = 0; k < network->num_layers; k++)
						{
							free(delta_weights[k]);
						}
						free(delta_weights);
						delta_weights = NULL;

					}

					//TODO: (potentially) Implement later
					//avg_error /= (float)no_training_data;
				}

				return 0;
			}


			int train_network_with_stats(float **desired, float **batch, std::int64_t batch_size,
				std::int64_t epochs, neural_network_t *network, std::int64_t *epochs_taken, float *avg_err, float target_err, error_energy err_func)
			{
				float avg_error = 1.f;
				std::int64_t epoch = 0;

				//float **delta_weights;

				float *output;

				while (avg_error > target_err && epoch < epochs)
				{

					for (int j = 0; j < batch_size; j++)
					{
						train_network(desired + j, batch + j, 1, 1, network);
						get_network_output(network, &output);

						avg_error += err_func(output, desired[j], 1);
						free(output);
					}

					avg_error /= batch_size;

					epoch++;
				}
				*epochs_taken = epoch;
				*avg_err = avg_error;
			}

			//TODO: decide wheter to change to string.
			void print_inferencing_results(neural_network_t *network,
				float **training_data, std::int64_t training_data_size, std::int64_t training_data_dimension,
				float **desired, std::int64_t desired_dimension)
			{
				//printf("no epochs: %i averaged error: %f\n", epoch, avg_error);

				//printf("--------------------------------\n");
				printf("Inferencing results:\n--------------------\n");

				float *output = NULL;

				for (std::int64_t j = 0; j < training_data_size; j++)
				{
					set_input(network, training_data[j]);
					float *desired_output = desired[j];

					feedforward(network);

					get_network_output(network, &output);

					printf("input: ( ");

					std::int64_t all_but_last_training_dim = training_data_dimension - 1;
					for (std::int64_t k = 0; k < all_but_last_training_dim; k++)
					{
						printf("%f, ", training_data[j][k]);
					}

					printf("%f ) desired: ( ", training_data[j][all_but_last_training_dim]);

					std::int64_t all_but_last_output_dim = desired_dimension - 1;
					for (std::int64_t k = 0; k < all_but_last_output_dim; k++)
					{
						printf("%f, ", desired[j][k]);
					}

					printf("%f ) -> Network Output : ( ", desired[j][all_but_last_output_dim]);

					for (std::int64_t k = 0; k < all_but_last_output_dim; k++)
					{
						printf("%f, ", output[k]);
					}

					printf("%f )\n", output[all_but_last_output_dim]);

					free(output);
				}
			}

			void get_network_output(neural_network_t *network, float **output)
			{
				if (network->tracking == NULL)
				{
					feedforward(network);
				}

				std::int64_t output_size = network->neuron_dims[network->num_layers - 1];

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
				for (std::int64_t i = 0; i < tracking->total_layer_count; i++)
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

			int copy_weights(float ***dst, float**src, std::int64_t num_layers, std::int64_t input_size, std::int64_t *neuron_dims)
			{
				*dst = (float**)malloc(sizeof(float*) * num_layers);

				if (*dst == NULL)
				{
					return -2;
				}

				std::int64_t prev_dimensions = input_size + 1;
				std::int64_t next_dimensions;
				std::int64_t weight_size;

				for (std::int64_t i = 0; i < num_layers; i++)
				{
					next_dimensions = neuron_dims[i];
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
					prev_dimensions = next_dimensions;
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

			*/

//} //namespace pulse