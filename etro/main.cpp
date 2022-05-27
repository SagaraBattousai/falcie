
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>


import pulse;

int x(int& a, int& b)
{
	return a < b ? -1 : 1;
}

int main(void)
{
	pulse::NeuralNetwork n({ 2, 3, 1 });

	//error somewhere but can't see where!!
	std::vector<std::vector<float>> desired{ {1.f}, {1.f}, {0.f}, {0.f} };
	std::vector<std::vector<float>> input{ {0.f,1.f}, {1.f,0.f}, {1.f,1.f}, {0.f, 0.f}};

	pulse::TrainNetwork(n, desired, input, 500);

	n.Feedforward(input[0]);
	float out1 = n.Output()[0];

	n.Feedforward(input[1]);
	float out2 = n.Output()[0];

	n.Feedforward(input[2]);
	float out3 = n.Output()[0];

	n.Feedforward(input[3]);
	float out4 = n.Output()[0];

	std::cout << out1 << ", " << out2 << ", " << out3 << ", " << out4 << std::endl;


	return 0;

}