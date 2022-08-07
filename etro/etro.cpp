#include <iostream>
#include <vector>
#include <cstdlib>
#include <anima/anima-matrix.h>
#include <utility>


int main()
{
	std::vector v{ 1,2,3 };
	pulse::Matrix m{ std::move(v) };

	m *= 3;

	int x = m[1];

	std::cout << *((int*)matrix_get_flat(m.Data(), 2, 4)) <<std::endl;

	std::cout << m[0] << " " << m[1] << " " << m[2] << std::endl;


	


	return 0;
}