
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

	NetworkUpdate combined_update;

	std::vector<float> w11{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<float> w12{ 10,11,12 };

	std::vector<float> w21{ 2, 4, 6, 8, 10, 12, 14, 16, 18 };
	std::vector<float> w22{ 24, 22, 20 };

	NetworkUpdate network1_update
	{
		.delta_weights = std::vector<pulse::Matrix<float>>{
			pulse::Matrix<float>(w11, {3,3}),
			pulse::Matrix<float>(w12, {3,1})
		},
		.examples_seen = 4
	};

	NetworkUpdate network2_update
	{
		.delta_weights = std::vector<pulse::Matrix<float>>{
			pulse::Matrix<float>(w21, {3,3}),
			pulse::Matrix<float>(w22, {3,1})
			},
		.examples_seen = 2
	};

	Federatedblock b = block_builder.Build();

	//b.AddLocalUpdate((std::vector<pulse::Matrix<float>>{
		//pulse::Matrix<float>(w11, { 3,3 }),
			//pulse::Matrix<float>(w12, { 3,1 })}), 4);
	
	b.AddLocalUpdate(network1_update);
	b.AddLocalUpdate(network2_update);

	//use ptrs and builder in next iter TODO:
	bc.Add(std::move(b));

	auto& last = bc.GetLast();

	auto& gu = last.GetGlobalUpdate();

	//network1.SetWeights(gu.delta_weights);
	//network2.SetWeights(gu.delta_weights);

	std::cout << gu.examples_seen << std::endl;

	std::cout << gu.delta_weights.size() << std::endl;

	for (auto& w1 : gu.delta_weights[0])
	{
		std::cout << w1 << " ";
	}

	std::cout << "\n\n";

	for (auto& w2 : gu.delta_weights[1])
	{
		std::cout << w2 << " ";
	}

	std::cout << std::endl;


	return 0;
}