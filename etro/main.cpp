
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

constexpr std::strong_ordering cmpt()
{
	pulse::Target x(0x1F0FFF1D); //{ pulse::Target::BaseTarget() };
    //std::vector<unsigned char> y { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
	pulse::Target y(0x0F0FFF1D); //{ pulse::Target::BaseTarget() };
	return x <=> y;

}


int main(void)
{

	pulse::Target x(0x1F0FFF1D); //{ pulse::Target::BaseTarget() };
	//constexpr pulse::Target y(0xFF0FFF1D); //{ pulse::Target::BaseTarget() };
	std::vector<unsigned char> y { 1,2,3,4,5,6,7,8,9,10,11,12,13 };

	//constexpr auto z = (x <=> y);
	//
	std::cout << (cmpt() == std::strong_ordering::greater) << (x > y) << std::endl;




	return 0;

}