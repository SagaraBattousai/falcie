
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <memory>

#include <chrono>
#include <span>
#include <array>

#include <cstddef>

#include <compare>


import pulse;

int main(void)
{
	using hash_type = std::vector<std::byte>;

	pulse::Blockheader b{};

	hash_type v = b.Hash();

	auto lambda = [](hash_type::pointer v)
	{
		for (auto i = 0; i < 32; i++)
			std::cout << std::hex << (unsigned)(v[i]);
	};

	lambda(v.data());

	std::cout << std::endl;

	pulse::Blockheader c{};

	c.Mine(b.Hash());

	hash_type w = c.Hash();

	//lambda(w.data());

	lambda(w.data());

	std::cout << std::endl;

	pulse::Blockheader d{};


	d.Mine(b.Hash());

	hash_type x = d.Hash();

	lambda(x.data());


	std::cout << std::endl;

	return 0;

}