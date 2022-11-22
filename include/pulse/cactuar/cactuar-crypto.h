#ifndef __CACTUAR_CRYPTO_H__
#define __CACTUAR_CRYPTO_H__

#include <vector>
#include <cstddef>
#include <span>

namespace pulse
{
	enum class HashAlgorithm
	{
		SHA256,
		RIPEMD160,

		/*
		 * This must be last->Since we start at 0 this will equal
		 * the total number of Algorithms offered (i.e. the number of
		 * elements in this enum other than this (which is a psudo-element).
		*/
		AvailiableHashAlgorithms

	};


	class CryptographicFunction
	{
	public:
		using return_type = std::vector<std::byte>;

		virtual return_type operator()(const std::span<const std::byte> input) const = 0;
	};

}

#endif // !__CACTUAR_CRYPTO_H__
