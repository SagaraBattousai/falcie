
#include <cstdint>
#include <iostream>


import <vector>;
import <utility>;

import pulse;
import lindzei;


int main(void)
{
	using lindzei::Federatedblock;
	using pulse::NeuralNetwork;
	using lindzei::NetworkUpdate;

	Federatedblock genisis = Federatedblock::Genisis();

	pulse::Blockchain<lindzei::Federatedblock, 32> bc{ std::move(genisis), 32 };

	Federatedblock b1{};

	bc.Add(std::move(b1));

	auto bb1 = bc.GetLast();

	for (auto i : bb1.Hash())
	{
		std::cout << std::hex << (int)i;
	}

	std::cout << std::endl;



	const std::vector<std::vector<float>> training_data{
		{ 0.f, 0.f }, {1.f, 0.f }, { 0.f, 1.f }, { 1.f, 1.f }
	};

	const std::vector<std::vector<float>> desired_outputs{
		{ 0.f }, { 1.f }, { 1.f }, { 0.f }
	};

	const std::vector<std::vector<float>> training1{ { 0.f, 0.f }, {1.f, 0.f } };


	const std::vector<std::vector<float>> desired1{ { 0.f }, { 1.f } };

	const std::vector<std::vector<float>> training2{ { 0.f, 1.f }, { 1.f, 1.f } };


	const std::vector<std::vector<float>> desired2{ { 1.f }, { 0.f } };

	float learning_rate = 0.9;

	int max_epochs = 50000;
	float target_error = 0.00001;

	std::vector<std::int64_t> network_structure{ 2, 3, 1 };

	//unsigned seed = 0x45000000;

	NeuralNetwork network1{
		network_structure,
		&pulse::sigmoid, &pulse::delta_sigmoid,
		learning_rate };// , seed };

	NeuralNetwork network2{
		network_structure,
		&pulse::sigmoid, &pulse::delta_sigmoid,
		learning_rate };// , seed };

	NeuralNetwork network_control{
		network_structure,
		&pulse::sigmoid, &pulse::delta_sigmoid,
		learning_rate }; // , seed };

	pulse::NetworkWeights weights1;
	pulse::NetworkWeights weights2;


	NetworkUpdate combined_update;

	Federatedblock b2{};

	for (int i = 0; i < 50000; i++)
	{
		pulse::TrainNetwork(network1, desired1, training1, 1);

		pulse::TrainNetwork(network2, desired2, training2, 1);

		pulse::TrainNetwork(network_control, desired_outputs, training_data, 1);

		weights1 = network1.GetWeights();
		weights2 = network2.GetWeights();

		NetworkUpdate network1_update
		{
			.network_structure = network_structure,
			.delta_weights = weights1,
			.examples_seen = 2
		};

		NetworkUpdate network2_update
		{
			.network_structure = network_structure,
			.delta_weights = weights2,
			.examples_seen = 2
		};

		Federatedblock b2{};

		b2.AddLocalUpdate(std::move(network1_update));
		b2.AddLocalUpdate(std::move(network2_update));

		bc.Add(std::move(b2));
		auto last = bc.GetLast();

		auto gu = last.GetGlobalUpdate();

		network1.SetWeights(gu.delta_weights);
		network2.SetWeights(gu.delta_weights);

	}

	std::cout << "\nNetwork 1 on all data:\n";

	pulse::print_inferencing_results(network1, training_data, desired_outputs);


	std::cout << "\nNetwork control on all data:\n";

	pulse::print_inferencing_results(network_control, training_data, desired_outputs);


	//print_inferencing_results(network1, training_ptr, 4,
	//	2, desired_ptr, 1);

	//printf("\nControl Network on all data:\n");
	//print_inferencing_results(network_control, training_ptr, 4,
	//	2, desired_ptr, 1);

	return 0;
}