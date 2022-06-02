
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
	using hash_type = std::array<std::byte, 32>;

	pulse::Blockheader<pulse::HashAlgorithm::SHA256> b{};

	hash_type v = b.Hash();

	for (auto &x : v)
	{
		std::cout << std::hex << (unsigned)x;
	}

	std::cout << std::endl;

	v = b.Hash();

	for (auto &x : v)
	{
		std::cout << std::hex << (unsigned)x;
	}

	std::cout << std::endl;

	pulse::Blockheader<pulse::HashAlgorithm::SHA256> c{};

	hash_type w = c.Hash();

	for (auto &x : w)
	{
		std::cout << std::hex << (unsigned)x;
	}

	std::cout << std::endl;

	return 0;

}