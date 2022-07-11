
#include <cstdint>
#include <iostream>

#include <vector> //import <vector>;
#include <utility> //import <utility>;

#include <pulse/pulse.h> //import pulse;
#include <lindzei/lindzei.h> //import lindzei;

int main(void)
{
	
	using lindzei::Federatedblock;
	using pulse::NeuralNetwork;
	using lindzei::NetworkUpdate;

	Federatedblock genisis = Federatedblock::Genisis();

	pulse::Blockchain<lindzei::Federatedblock, 32> bc{ std::move(genisis), 32 };

	//std::cout << "Genisis:\n" << genisis << "\n";

	Federatedblock block{};

	bc.Add(std::move(block));

	//std::cout << bc.GetLast() << std::endl;

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

	float learning_rate = 0.9f;

	//int max_epochs = 50000;
	//float target_error = 0.00001f;

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

	//0 & 0 & 0 & 0.0246 & 0.0452 \\
	//0 & 1 & 1 & 0.952 & 0.925 \\
	//1 & 0 & 1 & 0.950 & 0.931 \\
	//1 & 1 & 0 & 0.205 & 0.0813 \\[1ex]

	std::vector<float> n1_avg(training_data.size());
	std::vector<float> nc_avg(training_data.size());

	for (int avg_count = 0; avg_count < 97; avg_count++)
	{
		for (int i = 0; i < 500; i++)
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
			auto& last = bc.GetLast();

			auto&gu = last.GetGlobalUpdate();

			network1.SetWeights(gu.delta_weights);
			network2.SetWeights(gu.delta_weights);


		}

		std::vector<std::vector<float>> n1_inference = network1(training_data);
		for (int k = 0; k < n1_avg.size(); k++)
		{
			const auto &i = n1_inference[k];

			for (const auto& j : i) //Only 1 elem
			{
				n1_avg[k] += j;
			}
		}


		std::vector<std::vector<float>> nc_inference = network_control(training_data);
		for (int k = 0; k < nc_avg.size(); k++)
		{
			const auto &i = nc_inference[k];

			for (const auto& j : i) //Only 1 elem
			{
				nc_avg[k] += j;
			}
		}
	}

	///////add old runs
	n1_avg[0] += (0.065025 + 0.020389 + 0.050252);
	n1_avg[1] += (0.941512 + 0.934605 + 0.897963);
	n1_avg[2] += (0.936504 + 0.932636 + 0.922550);
	n1_avg[3] += (0.065485 + 0.076678 + 0.101760);


	nc_avg[0] += (0.021932 + 0.011712 + 0.040020);
	nc_avg[1] += (0.954518 + 0.958125 + 0.942102);
	nc_avg[2] += (0.949580 + 0.956799 + 0.942347);
	nc_avg[3] += (0.055183 + 0.049161 + 0.511960);


	for (auto& n1_a : n1_avg)
	{
		std::cout << n1_a << ", " << n1_a / 100 << std::endl;
	}

	for (auto& nc_a : nc_avg)
	{
		std::cout <<nc_a << ", " << nc_a / 100 << std::endl;
	}



		
		//std::cout << bc.GetLast() << std::endl;


	//std::cout << "\nNetwork 1 on all data:\n";

	//pulse::PrintInferencingResults(network1, training_data, desired_outputs);


	//std::cout << "\nNetwork control on all data:\n";

	//pulse::PrintInferencingResults(network_control, training_data, desired_outputs);

	return 0;
}