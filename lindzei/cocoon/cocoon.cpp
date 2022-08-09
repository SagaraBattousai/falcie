
#include <cstdint>
#include <iostream>

#include <vector> //import <vector>;
#include <utility> //import <utility>;

#include <pulse/pulse.h> //import pulse;
#include <lindzei/lindzei.h> //import lindzei;

int main(void)
{

	using lindzei::Federatedblock;
	//using pulse::NeuralNetwork;
	using lindzei::NetworkUpdate;

	Federatedblock::Builder block_builder = Federatedblock::Builder().WithVersion(0x02);

	pulse::Blockchain<lindzei::Federatedblock, 32> bc{ block_builder.Genisis(), 32 };

	//Shouldnt need anymore
	std::vector<std::int64_t> network_structure{ 2, 3, 1 };


	//pulse::NetworkWeights weights1;
	//pulse::NetworkWeights weights2;


	NetworkUpdate combined_update;

	std::vector<float> n1_avg(training_data.size());
	std::vector<float> nc_avg(training_data.size());


	//weights1 = network1.GetWeights();
	//weights2 = network2.GetWeights();

	/*
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
	*/
	Federatedblock block{};

	//block.AddLocalUpdate(std::move(network1_update));
	//block.AddLocalUpdate(std::move(network2_update));

	bc.Add(std::move(block));
	auto& last = bc.GetLast();

	auto& gu = last.GetGlobalUpdate();

	//network1.SetWeights(gu.delta_weights);
	//network2.SetWeights(gu.delta_weights);


	return 0;
}