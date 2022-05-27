module;

#include <cstdint>
#include <algorithm>
#include <random>
//#include <iostream>

#include <array>
#include <vector>
#include <functional>

export module anima:neural_network;

//import <array>;
//import <vector>;

import :matrix;
import :random;

/*******************
 * Exported Alias' *
 *******************/

export namespace pulse
{
	using ActivationFunction = float (*)(const float&);

	using ErrorEnergy = float (*)(float *, float *, std::int64_t);

	using NetworkWeights = std::vector<Matrix<float>>;


} // namespace pulse

namespace pulse
{
	namespace anima
	{
		constexpr float DEFAULT_NETWORK_LEARNING_RATE = 0.9;

		struct FeedForwardTracking
		{
			/** The value of every neuron after a pass through the network. */
			std::vector<std::vector<float>> layers;

			/** The same as layers but before the activation function is applied*/
			std::vector<std::vector<float>> pre_activation_layers;
		};

		void ResetTracking(FeedForwardTracking& tracking)
		{
			/*
			tracking.layers[0].clear();
			tracking.pre_activation_layers[0].clear();
			tracking.layers[0].push_back(1.f);
			tracking.pre_activation_layers[0].push_back(1.f);
			*/
			tracking.layers[0].erase(tracking.layers[0].begin() + 1, tracking.layers[0].end());
			tracking.pre_activation_layers[0].erase(
				tracking.pre_activation_layers[0].begin() + 1, tracking.pre_activation_layers[0].end());


			for (auto i = 1; i < tracking.layers.size(); i++)
			{
				tracking.layers[i].clear();
				tracking.pre_activation_layers[i].clear();
			}
		}

		FeedForwardTracking init_tracking(const std::vector<std::int64_t>& network_structure)
		{
			//Have to declare here first as when coppied as below we loose the capacity reservation.
			FeedForwardTracking tracking{
				.layers = std::vector<std::vector<float>>(network_structure.size()),
				.pre_activation_layers = std::vector<std::vector<float>>(network_structure.size())
			};

			auto it = network_structure.begin();
			std::int64_t input_layer_size = *it + 1; //plus 1 for the bias!

			//Only need reserve for first layer as rest use copy TODO: have a look
			tracking.layers[0].reserve(input_layer_size);
			tracking.pre_activation_layers[0].reserve(input_layer_size);

			for (auto i = 1; i < network_structure.size(); i++)
			{
				tracking.layers[i].reserve(*(it + i));
				tracking.pre_activation_layers[i].reserve(*(it + i));
			}

			//TODO TEST this theory but emblace back is unneccisary for primitives?
			//Add bias term.
			//layers[0].emplace_back(1.f);
			//pre_activation_layers[0].emplace_back(1.f);

			tracking.layers[0].push_back(1.f);
			tracking.pre_activation_layers[0].push_back(1.f);


			return tracking;// FeedForwardTracking{ layers, pre_activation_layers };
		}

	} //namespace pulse::anima
} //namespace pulse


// NOTE: Dont kill all C code as that is still needed for wrapper.
//Public api
export namespace pulse
{
	float sigmoid(const float&);

	float delta_sigmoid(const float&);

	class NeuralNetwork
	{
	public:

		NeuralNetwork(std::vector<std::int64_t> network_structure,
			ActivationFunction activation = &sigmoid,
			ActivationFunction delta_activation = &delta_sigmoid,
			float learning_rate = anima::DEFAULT_NETWORK_LEARNING_RATE,
			std::uint_fast32_t seed = std::random_device{}());

		/** Returns the number of layers in the network(minus the input layer) */
		constexpr std::vector<std::int64_t>::size_type NumberOfLayers();

		//constexpr ??? NetworkWeights& GetWeights() const;
		const NetworkWeights& GetWeights() const;

		void SetWeights(NetworkWeights weights);

		void UpdateWeights(const NetworkWeights& delta_weights);

		const std::vector<float>& Output() const;

		/** A Forward pass through the network. */
		void Feedforward(const std::vector<float>&);


		NetworkWeights Backpropagation(const std::vector<float>& desired);


		

		/*
		void print_inferencing_results(neural_network_t *network,
			float **training_data, std::int64_t training_data_size, std::int64_t training_data_dimension,
			float **desired, std::int64_t desired_dimension);*/

	private:

		//TODO: Make this configurable
		static constexpr int BACKPROP_ALPHA = 2;

		/** An array describing number of neurons in each layer.
		 *  Layer x has network_structure[x - 1] neurons.
		 */
		std::vector<std::int64_t> network_structure;

		/** An array of weights with length network_structure - 1 */
		NetworkWeights weights;

		//TODO: make into an activation array.
		/** A function pointer to the activation function for the network. */
		ActivationFunction activation;

		ActivationFunction delta_activation;

		float learning_rate;

		anima::FeedForwardTracking tracking;

		//std::uint_fast32_t seed;

	};

	//These three are just convieience (at least the last two are) so maybe make non-member-non friend?
		// batch has size batch size and batch[x] has input size size
	void TrainNetwork(
		NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired,
		const std::vector<std::vector<float>>& input,
		const std::int64_t& epochs); //is a ref larger/as large than by value for this??

	//for now use multy ret by reference but i think well wanna change this //TODO:
	// batch has size batch size and batch[x] has input size size
	void train_network_with_stats(
		NeuralNetwork& network,
		const std::vector<float>& desired,
		const std::vector<float>& input,
		const std::int64_t& epochs,
		float target_err,
		ErrorEnergy err_func,
		std::int64_t *epochs_taken,
		float *average_network_error);

	
	//Could return latest update?
	//These three are just convieience (at least the last two are) so maybe make non-member-non friend?
	// batch has size batch size and batch[x] has input size size
	void TrainNetwork(
		NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired,
		const std::vector<std::vector<float>>& input,
		const std::int64_t& epochs)
	{
		//Assert: input.size() == desired.size();

		for (int i = 0; i < epochs; i++)
		{
			for (std::int64_t j = 0; j < input.size(); j++)
			{
				//set_input(network, batch[j]);
				//float *desired_output = desired[j];

				network.Feedforward(input[j]);			
				NetworkWeights deltaWeights = network.Backpropagation(desired[j]);

				network.UpdateWeights(deltaWeights);
			}
		}
	}








} //namespace pulse