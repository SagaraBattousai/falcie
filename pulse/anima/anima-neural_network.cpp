
//module;

#include <cstdint>
#include <cmath>
//#include <algorithm>


//module anima:neural_network;

#include <array> //import <array>;
#include <vector> //import <vector>;
#include <random> //import <random>;

//#include <anima/anima-matrix.h> //import :matrix;
//#include <anima/anima-random.h> //import :random;
#include <anima/anima-neural_network.h>


namespace pulse
{

	namespace anima
	{
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

			for (std::int64_t i = 1; i < static_cast<std::int64_t>(network_structure.size()); i++)
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

		void ResetTracking(FeedForwardTracking& tracking)
		{
			tracking.layers[0].erase(
				tracking.layers[0].begin() + 1,
				tracking.layers[0].end()
			);

			tracking.pre_activation_layers[0].erase(
				tracking.pre_activation_layers[0].begin() + 1, tracking.pre_activation_layers[0].end()
			);

			for (std::int64_t i = 1; i < static_cast<std::int64_t>(tracking.layers.size()); i++)
			{
				tracking.layers[i].clear();
				tracking.pre_activation_layers[i].clear();
			}
		}

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
		for (std::int64_t i = 0; i < static_cast<std::int64_t>(output.size()); i++)
		{
			output[i] -= desired[i];
		}

		for (std::int64_t i = 0; i < static_cast<std::int64_t>(output.size()); i++)
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

	void NeuralNetwork::SetWeights(NetworkWeights newWeights)
	{
		this->weights = newWeights;
	}

	void NeuralNetwork::UpdateWeights(const NetworkWeights& delta_weights)
	{
		auto weights_iter = this->weights.begin();
		auto delta_iter = delta_weights.begin();
		//Assert delta_weights.size == this->weights.size
		for (std::int64_t i = 0; i < static_cast<std::int64_t>(delta_weights.size()); i++)
		{
			*(weights_iter + i) += *(delta_iter + i);
		}
	}

	const std::vector<float>& NeuralNetwork::Output() const
	{
		return this->tracking.layers[this->network_structure.size() - 1];
	}

	//Todo: make feedforward and this function call private base function that does both
	/** A Forward pass through the network without tracking. */
	std::vector<float> NeuralNetwork::operator()(const std::vector<float>& input)
	{
		this->Feedforward(input);

		return this->Output();
	}

	//Todo: make feedforward and this function call private base function that does both
	/** A Forward pass through the network without tracking for a set of inputs. */
	std::vector<std::vector<float>> NeuralNetwork::operator()(
		const std::vector<std::vector<float>>& inputs)
	{

		std::vector<std::vector<float>> inference_pass{};
		inference_pass.reserve(inputs.size());

		for (const auto& input : inputs)
		{
			this->Feedforward(input);

			inference_pass.push_back(this->Output());
		}

		return inference_pass;
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
				this->tracking.layers[i] *(this->weights[i]);// .Data();

			for (std::int64_t j = 0;
				j < static_cast<std::int64_t>(this->tracking.pre_activation_layers[i + 1].size());
				j++)
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

		for (std::int64_t j = this->NumberOfLayers(); j > 0; j--)
		{
			back_neuron_count = this->network_structure[j - 1];

			if (j == 1)
			{
				back_neuron_count += 1; //If network_structure is internal could add 1 in constructor.
			}

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

		for (std::int64_t i = 0; i < epochs; i++)
		{
			for (std::int64_t j = 0; j < static_cast<std::int64_t>(input.size()); j++)
			{
				network.Feedforward(input[j]);
				NetworkWeights deltaWeights = network.Backpropagation(desired[j]);

				network.UpdateWeights(deltaWeights);
			}
		}
	}

	NetworkStats TrainNetworkWithStats(NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired, const std::vector<std::vector<float>>& input,
		const std::int64_t epochs, float target_err, ErrorEnergy err_func)
	{
		NetworkStats stats{ .epochs_taken = 0, .average_network_error = 1.f };

		while (stats.average_network_error > target_err && stats.epochs_taken < epochs)
		{
			for (std::int64_t j = 0; j < static_cast<std::int64_t>(input.size()); j++)
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

	std::vector<float> GetInferenceError(NeuralNetwork& network,
		const std::vector<std::vector<float>>& desired, const std::vector<std::vector<float>>& input,
		ErrorEnergy err_func)
	{
		std::vector<float> inference_error{};
		inference_error.reserve(input.size());

		for (std::int64_t i = 0; i < static_cast<std::int64_t>(input.size()); i++)
		{
			network.Feedforward(input[i]);

			const std::vector<float>& output = network.Output();

			inference_error.push_back(err_func(output, desired[i]));
		}

		return inference_error;
	}

	//TODO: decide wheter to change to string.
	void PrintInferencingResults(NeuralNetwork& network,
		const std::vector<std::vector<float>>& input, const std::vector<std::vector<float>>& desired)
	{
		std::cout << "Inferencing results:\n--------------------\n";

		for (std::int64_t i = 0; i < static_cast<std::int64_t>(input.size()); i++)
		{
			network.Feedforward(input[i]);
			std::vector<float> output = network.Output();

			std::cout << "input: ( ";

			for (std::int64_t j = 0; j < static_cast<std::int64_t>(input[i].size()) - 1; j++)
			{
				std::cout << input[i][j] << ", ";
			}

			std::cout << input[i].back() << " ) desired: ( ";

			for (std::int64_t j = 0; j < static_cast<std::int64_t>(desired[i].size()) - 1; j++)
			{
				std::cout << desired[i][j] << ", ";
			}

			std::cout << desired[i].back() << " ) -> Network Output : ( ";

			for (std::int64_t j = 0; j < static_cast<std::int64_t>(output.size()) - 1; j++)
			{
				std::cout << output[j] << ", ";
			}

			std::cout << output.back() << " )" << std::endl;
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

