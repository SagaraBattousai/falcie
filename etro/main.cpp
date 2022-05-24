
#include <iostream>
#include <array>
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
	pulse::Matrix<int> m({1,2,3,4,5,6,7,8,9},  { 3, 3 });

	std::cout << m.TotalElementCount() << std::endl;

	return 0;

}