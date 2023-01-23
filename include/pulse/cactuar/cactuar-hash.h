//module;

#ifndef __CACTUAR_HASH__
#define __CACTUAR_HASH__

#include <cstddef>

//export module cactuar:hash;

#include <vector> //import <vector>;
#include <span> //import <span>;
#include <functional>

#include <cactuar/cactuar-crypto.h>

//export
namespace pulse
{

	int HashSize(HashAlgorithm hash_algo);

	class HashFunction : CryptographicFunction
	{
	public:

		using hash_function = 
			std::function<CryptographicFunction::return_type(CryptographicFunction::input_type)>;

		HashFunction(HashAlgorithm hash_algo);

		virtual CryptographicFunction::return_type
			operator()(CryptographicFunction::input_type input) const override
		{
			return hash_func(input);
		};

		friend HashFunction operator*(HashFunction&& lhs, HashFunction&& rhs); //Function Composition

	private:

		explicit HashFunction(hash_function hash_func);

		const hash_function hash_func; //shame it cant be auto

	};

} //namespace pulse

#endif