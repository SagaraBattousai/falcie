
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

#include<chrono>


import pulse;

int x(int& a, int& b)
{
	return a < b ? -1 : 1;
}

int main(void)
{
	
	std::vector<int> veccy{ 1,2,3 };
	constexpr int veccy_size = 3 * sizeof(int);

	auto start = std::chrono::high_resolution_clock::now();

	pulse::Hasher<pulse::HashType::SHA256> h{};

	auto d = h.Hash(veccy.data(), veccy_size);

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;

	std::cout << diff << std::endl;

	for (const auto& v : d)
	{
		std::cout << std::hex << (int)v;
	}

	std::cout << std::endl;


	start = std::chrono::high_resolution_clock::now();

	d = h.Hash(veccy.data(), veccy_size);

	end = std::chrono::high_resolution_clock::now();

	diff = end - start;

	std::cout << diff << std::endl;

	for (const auto& v : d)
	{
		std::cout << std::hex << (int)v;
	}

	std::cout << std::endl << std::endl;

	start = std::chrono::high_resolution_clock::now();

	pulse::Hasher<pulse::HashType::RIPEMD160> r{};

	auto c = r.Hash(veccy.data(), veccy_size);

	end = std::chrono::high_resolution_clock::now();

	diff = end - start;

	std::cout << diff << std::endl;

	for (const auto& v : c)
	{
		std::cout << std::hex << (int)v;
	}

	std::cout << std::endl;

	start = std::chrono::high_resolution_clock::now();

	c = r.Hash(veccy.data(), veccy_size);

	end = std::chrono::high_resolution_clock::now();

	diff = end - start;

	std::cout << diff << std::endl;

	for (const auto& v : c)
	{
		std::cout << std::hex << (int)v;
	}

	std::cout << std::endl;



	return 0;

}