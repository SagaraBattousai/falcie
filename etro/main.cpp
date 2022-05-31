
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

#include <chrono>
#include <span>
#include <array>

#include <cstddef>

#include <compare>


import pulse;

int main(void)
{

	pulse::Blockheader b{};

	std::vector<unsigned char> v = b.Hash();

	for (auto &x : v)
	{
		std::cout << std::hex << (unsigned)x;
	}

	std::cout << std::endl;

	std::cout << "acdd44d36dd2ee67a7a390b7393920feb1e1ea4f41fed8c91afbfe6c6de216\n";

	return 0;

}