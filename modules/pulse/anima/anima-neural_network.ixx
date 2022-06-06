module;

#include <cstdint>
#include <algorithm>
#include <random>
#include <iostream>



export module anima:neural_network;

import <array>;
import <vector>;
import <functional>;

import :matrix;
import :random;

/*******************
 * Exported Alias' *
 *******************/

export namespace pulse
{
	using ActivationFunction = float (*)(const float&);

	using ErrorEnergy = float (*)(std::vector<float>, const std::vector<float>&);

	using NetworkWeights = std::vector<Matrix<float>>;

} // namespace pulse

namespace pulse
{
	namespace anima
	{
		constexpr float DEFAULT_NETWORK_LEARNING_RATE = 0.9;

		//Make class with constructor!!
		struct FeedForwardTracking
		{
			/** The value of every neuron after a pass through the network. */
			std::vector<std::vector<float>> layers;

			/** The same as layers but before the activation function is applied*/
			std::vector<std::vector<float>> pre_activation_layers;
		};

		void ResetTracking(FeedForwardTracking& tracking)
		{
			tracking.layers[0].erase(
				tracking.layers[0].begin() + 1,
				tracking.layers[0].end()
			);

			tracking.pre_activation_layers[0].erase(
				tracking.pre_activation_layers[0].begin() + 1, tracking.pre_activation_layers[0].end()
			);

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


			return tracking;
		}

	} //namespace pulse::anima
} //namespace pulse


export namespace pulse
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
		constexpr inline std::vector<std::int64_t>::size_type NumberOfLayers()
		{
			return this->network_structure.size() - 1;
		}

		//constexpr ??? NetworkWeights& GetWeights() const;
		NetworkWeights GetWeights() const;

		void SetWeights(NetworkWeights weights);

		void UpdateWeights(const NetworkWeights& delta_weights);

		const std::vector<float>& Output() const;

		constexpr std::int64_t InputSize() const;

		constexpr std::int64_t OutputSize() const;

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


	void NeuralNetwork::Feedforward(const std::vector<float>& input)
	{
		anima::ResetTracking(this->tracking);
		//std::int64_t number_of_layers = (this->network_structure[0] + 1); // +1 for bias
		//^^ Would never have worked, what the heck is this

		//Assert: input.size() == this->network_structure[0]

		for (auto it = input.begin(); it != input.end(); ++it)
		{
			//Remember tracking already has the bias term added in
			this->tracking.layers[0].push_back(*it);
			this->tracking.pre_activation_layers[0].push_back(*it);
		}

		for (std::int64_t i = 0; i < this->NumberOfLayers(); i++)
		{
			//TODO: Check this doesn't die when leaving the function (or does = call copy constructor ?
			this->tracking.pre_activation_layers[i + 1] =
				(Matrix{ this->tracking.layers[i] } * (this->weights[i])).Data();

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

		for (std::int64_t j = this->NumberOfLayers(); j > 0; j--)
		{
			curr_neuron_count = this->network_structure[j];

			back_neuron_count = this->network_structure[j - 1];

			if (j == 1)
			{
				back_neuron_count += 1; //If network_structure is internal could add 1 in constructor.
			}

			weight_size = (back_neuron_count * curr_neuron_count);

			//weightupdate = curlayers[l] * pdelta.T
			deltaWeights.insert(deltaWeights.begin(), 
				Broadcast<float, float, float, std::multiplies<float>>
				(this->tracking.layers[j - 1], delta, std::multiplies<float>())
			);

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


	NetworkStats TrainNetworkWithStats(NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired, const std::vector<std::vector<float>>& input,
		const std::int64_t epochs, float target_err, ErrorEnergy err_func = &TotalSquaredErrorEnergy)
	{
		NetworkStats stats{ .epochs_taken = 0, .average_network_error = 1.f };

		while (stats.average_network_error > target_err && stats.epochs_taken < epochs)
		{
			for (int j = 0; j < input.size(); j++)
			{
				TrainNetwork(network, { desired[j] }, { input[j] }, 1);

				const std::vector<float>& output = network.Output();

				stats.average_network_error += err_func(output, desired[j]);
			}
			stats.average_network_error /= input.size();

			stats.epochs_taken++;
		}
		return stats;
	}

	//TODO: decide wheter to change to string.
	void print_inferencing_results(NeuralNetwork& network,
		const std::vector<std::vector<float>>& input, const std::vector<std::vector<float>>& desired)
	{
		std::cout << "Inferencing results:\n--------------------\n";

		for (std::int64_t i = 0; i < input.size(); i++)
		{
			network.Feedforward(input[i]);
			std::vector<float> output = network.Output();

			std::cout << "input: ( ";

			for (std::int64_t j = 0; j < input[i].size() - 1; j++)
			{
				std::cout << input[i][j] << ", ";
			}

			std::cout << input[i].back() << " ) desired: ( ";

			for (std::int64_t j = 0; j < desired[i].size() - 1; j++)
			{
				std::cout << desired[i][j] << ", ";
			}

			std::cout << desired[i].back() << " ) -> Network Output : ( ";

			for (std::int64_t j = 0; j < output.size() - 1; j++)
			{
				std::cout << output[j] << ", ";
			}

			std::cout << output.back() << " )" << std::endl;
		}
	}
} //namespace pulse