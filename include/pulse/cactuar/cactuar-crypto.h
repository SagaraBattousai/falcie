#ifndef __CACTUAR_CRYPTO_H__
#define __CACTUAR_CRYPTO_H__

#include <vector>
#include <cstddef>
#include <ostream>
#include <span>

namespace pulse
{
	template<typename T> //std::is_scoped_enum<T>(); //Only avaliable in C++23
	constexpr int GetEnumCount()
	{
		return static_cast<int>(T::COUNT);
	}

	template<typename T>
	concept CrytographicTypeEnum = requires
	{
		// std::vector<std::array<void, 1>>::size_type > ;
		{GetEnumCount<T>()} -> std::convertible_to<int>;
	};

	enum class HashAlgorithm
	{
		SHA256,
		RIPEMD160,

		/*
		 * This must be last->Since we start at 0 this will equal
		 * the total number of Algorithms offered (i.e. the number of
		 * elements in this enum other than this (which is a psudo-element).
		*/
		COUNT
	};

	std::ostream& operator<<(std::ostream&, const HashAlgorithm&);

	enum class ElipticalCurve
	{
		SECP256K1,

		/*
		 * This must be last->Since we start at 0 this will equal
		 * the total number of Curves offered (i.e. the number of
		 * elements in this enum other than this (which is a psudo-element).
		*/
		COUNT
	};

	std::ostream& operator<<(std::ostream&, const ElipticalCurve&);


	class CryptographicFunction
	{
	public:
		using return_type = std::vector<std::byte>;

		using input_type = const std::span<const std::byte>;

		virtual return_type operator()(input_type input) const = 0;
	};

}

#endif // !__CACTUAR_CRYPTO_H__
