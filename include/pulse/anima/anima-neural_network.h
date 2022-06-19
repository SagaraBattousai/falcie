
//module;
#ifndef __ANIMA_NEURAL_NETWORK__
#define __ANIMA_NEURAL_NETWORK__

#include <cstdint>
#include <algorithm>
#include <random>
#include <iostream>

//export module anima:neural_network;

#include <array> //import <array>;
#include <vector> //import <vector>;
#include <functional> //import <functional>;

#include <anima/anima-matrix.h> //import :matrix;
#include <anima/anima-random.h> //import :random;

/*******************
 * Exported Alias' *
 *******************/

//export 
namespace pulse
{
	using ActivationFunction = float (*)(const float&);

	using ErrorEnergy = float (*)(std::vector<float>, const std::vector<float>&);

	using NetworkWeights = std::vector<Matrix<float>>;

} // namespace pulse

namespace pulse
{
	namespace anima
	{
		constexpr float DEFAULT_NETWORK_LEARNING_RATE = 0.9f;

		//Make class with constructor!!
		struct FeedForwardTracking
		{
			/** The value of every neuron after a pass through the network. */
			std::vector<std::vector<float>> layers;

			/** The same as layers but before the activation function is applied*/
			std::vector<std::vector<float>> pre_activation_layers;
		};

		void ResetTracking(FeedForwardTracking& tracking);

		FeedForwardTracking init_tracking(const std::vector<std::int64_t>& network_structure);

	} //namespace pulse::anima
} //namespace pulse


//export 
namespace pulse
{
	float sigmoid(const float&);

	float delta_sigmoid(const float&);

	float TotalSquaredErrorEnergy(std::vector<float>, const std::vector<float>&);

	//Copy for outut so we can use as output
	float TotalSquaredErrorEnergy(std::vector<float> output, const std::vector<float>& desired);

	class NeuralNetwork
	{
	public:

		NeuralNetwork(std::vector<std::int64_t> network_structure,
			ActivationFunction activation = &sigmoid,
			ActivationFunction delta_activation = &delta_sigmoid,
			float learning_rate = anima::DEFAULT_NETWORK_LEARNING_RATE,
			std::uint_fast32_t seed = std::random_device{}());

		/** Returns the number of layers in the network(minus the input layer) */
		constexpr inline std::int64_t NumberOfLayers()
		{
			return this->network_structure.size() - 1;
		}

		//constexpr ??? NetworkWeights& GetWeights() const;
		NetworkWeights GetWeights() const;

		void SetWeights(NetworkWeights weights);

		void UpdateWeights(const NetworkWeights& delta_weights);

		/** A Forward pass through the network with tracking. */
		void Feedforward(const std::vector<float>&);

		//doesn't need to be member yet but will once we move Output (below)
		//Could make universal ref ....
		/** A Forward pass through the network without tracking. */
		std::vector<float> operator()(const std::vector<float>&);

		/** A Forward pass through the network without tracking for a set of inputs. */
		std::vector<std::vector<float>> operator()(const std::vector<std::vector<float>>&);

		const std::vector<float>& Output() const;

		constexpr std::int64_t InputSize() const;

		constexpr std::int64_t OutputSize() const;

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

	};

	void TrainNetwork(
		NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired,
		const std::vector<std::vector<float>>& input,
		const std::int64_t epochs);


	struct NetworkStats
	{
		std::int64_t epochs_taken;
		float average_network_error;
	};

	//for now use multy ret by reference but i think well wanna change this //TODO:
	// batch has size batch size and batch[x] has input size size
	NetworkStats TrainNetworkWithStats(
		NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired,
		const std::vector<std::vector<float>>& input,
		const std::int64_t epochs,
		float target_err,
		ErrorEnergy err_func
	);

	constexpr std::int64_t NeuralNetwork::InputSize() const
	{
		return this->network_structure.front();
	}

	constexpr std::int64_t NeuralNetwork::OutputSize() const
	{
		return this->network_structure.back();

	}

	NetworkStats TrainNetworkWithStats(NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired, const std::vector<std::vector<float>>& input,
		const std::int64_t epochs, float target_err, ErrorEnergy err_func = &TotalSquaredErrorEnergy);


	std::vector<float> GetInferenceError(NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired, const std::vector<std::vector<float>>& input,
		ErrorEnergy err_func);


	//TODO: decide wheter to change to string.
	void PrintInferencingResults(NeuralNetwork& network,
		const std::vector<std::vector<float>>& input, const std::vector<std::vector<float>>& desired);


} //namespace pulse

#endif