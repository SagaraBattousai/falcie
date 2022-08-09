#ifndef __ANIMA_DIMENSIONS_H__
#define __ANIMA_DIMENSIONS_H__

#include <cstdint>
#include <vector>
#include <initializer_list>
#include <numeric>
#include <functional>

namespace pulse
{
	class Dimensions
	{
	public:
		using value_type = std::int64_t;		

		Dimensions(std::initializer_list<value_type> dimensions);
		Dimensions(std::vector<value_type> dimensions);

		constexpr value_type FlattenIndex(const std::vector<value_type>& indicies) const;


		constexpr value_type TotalSize() const
		{
			return std::reduce(this->dims.begin(), this->dims.end(), (value_type) 1, std::multiplies<value_type>());

		}

	private:
		std::vector<value_type> dims;

		constexpr std::vector<value_type> DimensionIndex() const;

	}; 

} //namespace pulse


#endif