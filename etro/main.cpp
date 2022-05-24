
#include <iostream>
#include <array>
#include <algorithm>


import pulse;

int x(int& a, int& b)
{
	return a < b ? -1 : 1;
}

int main(void)
{
	pulse::NeuralNetwork<2, 1, 2> network{ std::array<std::int64_t, 2>{2, 1} };

	//This will be wrong #BUG
	pulse::Weights<2> w = network.GetWeights();

	std::cout << w[0][0] << ", " << w[0][1] << "\n" << w[1][0] << std::endl;

	std::vector<int>vec(5, 3);
	std::cout << vec.size() << std::endl;
	vec.push_back(7);
	//constexpr int cs = vec.size();
	std::cout << vec.size() << std::endl;// ", " << cs << std::endl;


	return 0;

}