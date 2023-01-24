
#include <utility>
#include <functional>
#include <numeric>

#include<anima/anima-dimensions.h>

namespace pulse
{

	Dimensions::Dimensions(std::initializer_list<value_type> dimensions)
		: dims(dimensions)
	{
	}

	Dimensions::Dimensions(std::vector<value_type> dimensions)
		: dims(std::move(dimensions))
	{
	}

	//constexpr std::vector<Dimensions::value_type> Dimensions::DimensionIndex() const
	

	/*
	constexpr Dimensions::value_type 
		Dimensions::FlattenIndex(const std::vector<value_type>& indicies) const
	{
		std::vector<std::int64_t> dimensionIndexer = this->DimensionIndex();

		//By using indicies for begining and end it allows shorter indicies since dims includes all dims ...
		return std::inner_product(indicies.begin(), indicies.end(),
			dimensionIndexer.begin(), (value_type) 0);

	}
	*/
	/*
	constexpr Dimensions::value_type Dimensions::TotalSize() const
	{
		return std::reduce(this->dims.begin(), this->dims.end(), 1, std::multiplies<value_type>());
	}
	*/

} //namespace pulse