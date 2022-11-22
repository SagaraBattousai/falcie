//module;

#ifndef __CACTUAR_HASH__
#define __CACTUAR_HASH__

#include <cstddef>

//export module cactuar:hash;

#include <vector> //import <vector>;
#include <array> //import <array>;
#include <algorithm> //import <algorithm>;
#include <span> //import <span>;
#include <memory> //import <memory>;
#include <map> //import <map>;
#include <ostream>
#include <functional>

#include <cactuar/cactuar-crypto.h>

//export
namespace pulse
{

	int HashSize(HashAlgorithm hash_algo);

	std::ostream& operator<<(std::ostream&, const HashAlgorithm);

	class HashFunction : CryptographicFunction
	{
	public:

		using hash_type = CryptographicFunction::return_type;

		using input_type = const std::span<const std::byte>;

		using hash_function = std::function<hash_type(input_type)>;

		HashFunction(HashAlgorithm hash_algo);

		virtual hash_type operator()(input_type input) const override
		{
			return hash_func(input);
		};

		friend HashFunction operator*(HashFunction&& lhs, HashFunction&& rhs); //Function Composition

	private:

		explicit HashFunction(hash_function hash_func);

		//const HashAlgorithm hash_algo;

		const hash_function hash_func; //shame it cant be auto

	};

} //namespace pulse

#endif