
#ifndef __ANIMA_RANDOM__
#define __ANIMA_RANDOM__

//export module anima:random;

#include <random> //import <random>;

namespace pulse
{
	namespace anima
	{
		template <typename RealType = float>
		RealType inverse_logistic_cdf(RealType x, RealType location = 0, RealType scale = 1,
			RealType min = 0, RealType max = 1)
		{
			//may not need cast as long as operator/ can handle it.
			RealType inv = location - scale * std::log(((RealType)1 / x) - 1);

			RealType re_range = std::fmod(inv, ((max - min) / (RealType)2));

			return re_range + ((max + min) / (RealType)2);
		}
	}
}

//export
namespace pulse
{
	template<typename RealType = float>
	class logistic_distribution
	{
	public:
		//Doubt they all have to be explicit but dont have time to worry
		//[min, max)
		explicit logistic_distribution();
		explicit logistic_distribution(RealType location, RealType scale);

		explicit logistic_distribution(RealType location, RealType scale, RealType min, RealType max);

		//Don't really get what this does but it doesn't actuall matter :D
		void reset();

		// generating functions
		template<typename Generator>
		RealType operator()(Generator& g);


		// property functions
		RealType Location() const;
		RealType Scale() const;
		RealType Min() const;
		RealType Max() const;

		// inserters and extractors
		//template<class charT, class traits>
		//friend basic_ostream<charT, traits>&
			//operator<<(basic_ostream<charT, traits>& os, const normal_distribution& x);
		//template<class charT, class traits>
		//friend basic_istream<charT, traits>&
			//operator>>(basic_istream<charT, traits>& is, normal_distribution& x);

	private:

		std::uniform_real_distribution<RealType> uniform_dist;

		RealType location;
		RealType scale;
		RealType min;
		RealType max;
	};

	//(min, max)
	template<typename RealType>
	logistic_distribution<RealType>::logistic_distribution()
		: logistic_distribution(0, 1, 0, 1)
	{
	}

	//(min, max)
	template<typename RealType>
	logistic_distribution<RealType>::logistic_distribution(RealType location, RealType scale)
		: logistic_distribution(location, scale, 0, 1)
	{
	}

	//(min, max)
	template<typename RealType>
	logistic_distribution<RealType>::logistic_distribution(
		RealType location, RealType scale,
		RealType min, RealType max)
		:location(location)
		, scale(scale)
		, min{ min }
		, max{ max }
		, uniform_dist(std::uniform_real_distribution<RealType>(0, 1))
	{}

	//Don't really get what this does but it doesn't actuall matter :D
	template<typename RealType>
	void logistic_distribution<RealType>::reset()
	{
		this->uniform_dist.reset();
	}

	template<typename RealType>
	template<typename Generator>
	RealType logistic_distribution<RealType>::operator()(Generator& g)
	{
		RealType cdf = this->uniform_dist(g);
		return anima::inverse_logistic_cdf(cdf, this->location, this->scale, this->min, this->max);
	}


	// property functions
	template<typename RealType>
	RealType logistic_distribution<RealType>::Location() const
	{
		return this->location;
	}

	template<typename RealType>
	RealType logistic_distribution<RealType>::Scale() const
	{
		return this->scale;
	}

	template<typename RealType>
	RealType logistic_distribution<RealType>::Min() const
	{
		return this->min;
	}

	template<typename RealType>
	RealType logistic_distribution<RealType>::Max() const
	{
		return this->max;
	}

} //namespace pulse

#endif