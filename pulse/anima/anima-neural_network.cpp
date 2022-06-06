
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
			weights.reserve(network_dimensions.size() - 1);
			

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

				//Or do I use move??
				weights.emplace_back(weight_values, Dimensions{ previous, next });
				weight_values.clear();

				previous = next;
			}
			return weights;
		}
	} //namespace pulse::anima

	float sigmoid(const float& pre_act)
	{
		return 1.f / (1 + std::exp(2.f * -pre_act));
	}

	float delta_sigmoid(const float& x)
	{
		float sig = sigmoid(x);
		return 2.f * sig * (1 - sig);
	}

	//Copy for outut so we can use as output
	float TotalSquaredErrorEnergy(std::vector<float> output, const std::vector<float>& desired)
	{
		float ret = 0;
		//TODO: Remove vector sub cheat!!!
		for (auto i = 0; i < output.size(); i++)
		{
			output[i] -= desired[i];
		}

		for (auto i = 0; i < output.size(); i++)
		{
			ret += output[i] * output[i];
		}

		return ret / 2.f;

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

	NetworkWeights NeuralNetwork::GetWeights() const
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


	

	//Could return latest update?
	//These three are just convieience (at least the last two are) so maybe make non-member-non friend?
	// batch has size batch size and batch[x] has input size size
	void TrainNetwork(
		NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired,
		const std::vector<std::vector<float>>& input,
		const std::int64_t epochs)
	{
		//Assert: input.size() == desired.size();

		for (int i = 0; i < epochs; i++)
		{
			for (std::int64_t j = 0; j < input.size(); j++)
			{
				network.Feedforward(input[j]);
				NetworkWeights deltaWeights = network.Backpropagation(desired[j]);

				network.UpdateWeights(deltaWeights);
			}
		}
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

