#include <iostream>
#include <anima/anima-matrix.h>
#include <cstdlib>
#include <vector>
#include <span>
#include <algorithm>

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
	std::vector<float> v{1, 2, 3, 4, 5, 6};
	pulse::Matrix<float> m{ {v.data(), 6}, {3,3} }; //Dims > data_size

	//pulse::Dimensions d{ 2,3 };

	//std::cout << d.TotalSize() << std::endl;

	print_span<float>({m.Data(), 9});

	//m *= 3;

	//print_span<float>({ m.Data(), 9 });


	return 0;

}