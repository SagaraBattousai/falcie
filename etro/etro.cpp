#include <iostream>
#include <anima/anima-matrix.h>
#include <cstdlib>
#include <vector>
#include <span>
#include <algorithm>
#include <utility>
#include <ranges>
#include <array>

template <typename T>
void print_span(std::span<T> v)
{
	for (auto i : v)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main()
{
	std::vector<float> v{ 1, 2, 3, 4, 5, 6 };
	pulse::Matrix<float> m{ {v.data(), 6}, {3,3} }; //Dims > data_size

	//pulse::Dimensions d{ 2,3 };

	//std::cout << d.TotalSize() << std::endl;

	print_span<float>(m);

	std::ranges::for_each(m, [](float& x) { x *= 3; });

	print_span<float>(m);

	pulse::Matrix<float> k = m * (float(1) / 3);

	print_span<float>(k);

	std::array<int, 3> x{};

	std::cout << x[0];

	k += m;

	print_span<float>(k);




	//m *= 3;

	//print_span<float>({ m.Data(), 9 });


	return 0;

}