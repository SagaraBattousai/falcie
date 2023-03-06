#ifndef __ANIMA_DIMENSIONS_H__
#define __ANIMA_DIMENSIONS_H__

#include <cstdint>
#include <vector>
#include <initializer_list>
#include <numeric>
#include <functional>

namespace anima
{
	class Dimensions
	{
	public:
		using value_type = std::int64_t;		

		Dimensions(std::initializer_list<value_type> dimensions) :dims(dimensions)
		{	}

		Dimensions(std::vector<value_type> dimensions) :dims(std::move(dimensions))
		{ }

		constexpr value_type FlattenIndex(const std::vector<value_type>& indicies) const
		{
			std::vector<std::int64_t> dimensionIndexer = this->DimensionIndex();

			//By using indicies for begining and end it allows shorter indicies since dims includes all dims ...
			return std::inner_product(indicies.begin(), indicies.end(),
				dimensionIndexer.begin(), (value_type)0);
		}

		constexpr value_type FlattenIndex(const Dimensions& indicies) const
		{
			return this->FlattenIndex(indicies.dims);
		}



		constexpr value_type TotalSize() const
		{
			return std::reduce(this->dims.begin(), this->dims.end(), (value_type) 1, std::multiplies<value_type>());
		}

		constexpr value_type DimensionCount() const
		{
			return dims.size();
		}

		constexpr const value_type* AsArray() const
		{
			return dims.data();
		}

	private:
		std::vector<value_type> dims;

		constexpr std::vector<value_type> DimensionIndex() const
		{
			std::vector<value_type> dimensionIndexer(this->dims.size());

			std::partial_sum(
				dims.rbegin(), dims.rend() - 1,
				dimensionIndexer.rbegin() + 1, std::multiplies<value_type>()
			);

			dimensionIndexer[dimensionIndexer.size() - 1] = 1;

			return dimensionIndexer;
		}

	}; 

} //namespace anima


#endif