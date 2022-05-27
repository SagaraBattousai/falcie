
module;

#include <cstdint>
#include <cmath>
//#include <algorithm>

#include <array>
#include <vector>
#include <random>


module anima:neural_network;

//import <array>;
//import <vector>;
//import <random>;

import :matrix;
import :random;


namespace pulse
{

	namespace anima
	{
		NetworkWeights GenerateWeights(
			const std::vector<std::int64_t>& network_dimensions,
			std::uint_fast32_t seed
		)
		{
			NetworkWeights weights{};
			//TODO: Add this line
			//weights.reserve
			// 
			//TODO: Change
			std::mt19937 bitgen{ seed };

			//logistic_distribution<float> randGen{};
			std::normal_distribution<float> randGen{};

			std::int64_t largest = *std::max_element(network_dimensions.begin(), network_dimensions.end());

			//Matrix<float> weight;
			std::vector<float> weight_values{};
			weight_values.reserve(largest * largest);

			//std::int64_t i = 0;
			std::int64_t previous;
			std::int64_t next;
			std::int64_t total_size;

			auto it = network_dimensions.begin();
			previous = *it + 1; // +1 for bias
			++it;

			for (; it != network_dimensions.end(); ++it)
			{
				next = *it;

				total_size = previous * next;

				for (std::int64_t i = 0; i < total_size; i++)
				{
					weight_values.push_back(randGen(bitgen));
				}

				weights.emplace_back(weight_values, Dimensions{ previous, next });
				weight_values.clear();

				previous = next;
			}
			return weights;
		}
	} //namespace pulse::anima

	float sigmoid(const float& pre_act)
	{
		return 1.f / (1 + std::exp(2.f *-pre_act));
	}

	float delta_sigmoid(const float& x)
	{
		float sig = sigmoid(x);
		return 2.f * sig * (1 - sig);
	}

	NeuralNetwork::NeuralNetwork(
		std::vector<std::int64_t> network_structure,
		ActivationFunction activation,
		ActivationFunction delta_activation,
		float learning_rate,
		std::uint_fast32_t seed)
		: network_structure(network_structure)
		, weights(anima::GenerateWeights(network_structure, seed))
		, activation(activation)
		, delta_activation(delta_activation)
		, learning_rate(learning_rate)
		, tracking(anima::init_tracking(network_structure))
	{
	}

	constexpr std::vector<std::int64_t>::size_type NeuralNetwork::NumberOfLayers()
	{
		return this->network_structure.size() - 1;
	}


	const NetworkWeights& NeuralNetwork::GetWeights() const
	{
		return this->weights;
	}

	void NeuralNetwork::SetWeights(NetworkWeights weights)
	{
		this->weights = weights;
	}

	void NeuralNetwork::UpdateWeights(const NetworkWeights& delta_weights)
	{
		auto weights_iter = this->weights.begin();
		auto delta_iter = delta_weights.begin();
		//Assert delta_weights.size == this->weights.size
		for (auto i = 0; i < delta_weights.size(); i++)
		{
			*(weights_iter + i) += *(delta_iter + i);
		}
	}

	const std::vector<float>& NeuralNetwork::Output() const
	{
		return this->tracking.layers[this->network_structure.size() - 1];
	}


	void NeuralNetwork::Feedforward(const std::vector<float>& input)
	{
		anima::ResetTracking(this->tracking);
		std::int64_t number_of_layers = (this->network_structure[0] + 1); // +1 for bias

		//Assert: input.size() == this->network_structure[0]

		//auto layers_iter = this->tracking.layers[0].begin();
		//auto pre_activation_layers_iter = this->tracking.pre_activation_layers[0].begin();


		//Set first tracking layer to input (bias term already added :) )

		//Assuming I can't do copy as size is only 1
		//Start at 1 as [0] is bias term
		for (auto it = input.begin(); it != input.end(); ++it)
		{
			//this->tracking.pre_activation_layers[0].emplace_back(*it);
			this->tracking.layers[0].push_back(*it);
			this->tracking.pre_activation_layers[0].push_back(*it);
		}

		//VV USe when using xor_answer_weights
	//memcpy(prev_layer, network->input, (sizeof(float) * network->input_size));
	//*(prev_layer + network->input_size) = 1.f; //The bias term


	//float *thislayer;
	//float *bthislayer;

	//-1 as weights has 1 less length than tracking
		for (std::int64_t i = 0; i < number_of_layers - 1; i++)
		{
			//TODO: Check this doesn't die when leaving the function (or does = call copy constructor ?
			this->tracking.pre_activation_layers[i + 1] =
				//(Matrix{ this->tracking.pre_activation_layers[i] } *(this->weights[i])).Data();
				(Matrix{ this->tracking.layers[i] } *(this->weights[i])).Data();

			//How come it worked before but now had compiler err??
			for (std::int64_t j = 0; j < this->tracking.pre_activation_layers[i + 1].size(); j++)
			{
				this->tracking.layers[i + 1].push_back(
					this->activation(this->tracking.pre_activation_layers[i + 1][j])
				);
			}
		}
	}

	NetworkWeights NeuralNetwork::Backpropagation(const std::vector<float>& desired)
	{
		NetworkWeights deltaWeights{};
		deltaWeights.reserve(this->network_structure.size() - 1);


		std::int64_t largest = *std::max_element(
			this->network_structure.begin(),
			this->network_structure.end()
		);

		std::vector<float> delta{};
		std::vector<float> nextDelta{};
		std::vector<float> delta_update{};
		delta.reserve(largest);
		nextDelta.reserve(largest);
		delta_update.reserve(largest);

		//TODO: could use iterators
		std::vector<float> pre_activation_output = this->tracking.pre_activation_layers.back();
		std::vector<float> output = this->tracking.layers.back();

		for (std::int64_t i = 0; i < this->network_structure.back(); i++)
		{
			// Alpha * output[i] * (1 - output[i]) * (desired[i] - output[i]);
			delta.push_back(//2 * output[i] * (1 - output[i]) * (desired[i] - output[i]));
				this->delta_activation(pre_activation_output[i]) * (desired[i] - output[i])
			);
		}

		// The neuron count for the next layer, which is actually
		// the previous layer in the network, as we are backtracking.
		// i.e. back_neurons <- curr_neurons.
		std::int64_t back_neuron_count;
		std::int64_t curr_neuron_count;
		std::int64_t weight_size;

		for (std::int64_t j = this->network_structure.size() - 1; j > 0; j--)
		{
			curr_neuron_count = this->network_structure[j];

			back_neuron_count = this->network_structure[j - 1];

			if (j == 1)
			{
				back_neuron_count += 1; //If network_structure is internal could add 1 in constructor.
			}

			weight_size = (back_neuron_count * curr_neuron_count);

			//weightupdate = curlayers[l] * pdelta.T
			deltaWeights.insert(deltaWeights.begin(), Broadcast<float, float, float, std::multiplies<float>>(this->tracking.layers[j - 1], delta, std::multiplies<float>()));

			//learning_rate * weight_update
			//[0] because we keep inserting at the head of the vector
			deltaWeights[0] *= this->learning_rate;

			if (j != 1) //Because there would be no need to calculte delta @j==1 aka last iter
			{

				//delta=aconst*curlayers[l]*(1-curlayers[l])*(np.dot(delta,weights[l]));
				for (std::int64_t i = 0; i < back_neuron_count; i++)
				{
					//  = Alpha * output[i] * (1 - output[i])
					nextDelta.push_back(
					// 2 * this->tracking.layers[j - 1][i] *
					//	(1 - this->tracking.layers[j -1][i]));
						this->delta_activation(
							this->tracking.pre_activation_layers[j - 1][i]
						)
					);
				}

				delta_update = this->weights[j - 1] * delta;

				//TODO: Probably don't need
				delta.clear();

				delta = nextDelta * delta_update;

				nextDelta.clear();
				delta_update.clear();
			}
		}
		return deltaWeights;
	}

	
} //namespace pulse



	//for now use multy ret by reference but i think well wanna change this //TODO:
	// batch has size batch size and batch[x] has input size size
/*	void train_network_with_stats(
		const std::vector<float>& desired,
		const std::vector<float>& input,
		const std::int64_t& epochs,
		float target_err,
		ErrorEnergy err_func,
		std::int64_t *epochs_taken,
		float *average_network_error);
		*/

		/*
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
		*/
