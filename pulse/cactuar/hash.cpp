//module;

#include <utility>
//module cactuar:hash;
#include <cactuar/cactuar-hash.h>
#include <cactuar/cactuar-hash_pool.h>
#include <cactuar/internal/cactuar-openssl_wrapper.h>

namespace pulse
{
	std::ostream& operator<<(std::ostream& os, const HashAlgorithm hashAlgo)
	{
		switch (hashAlgo)
		{
		case HashAlgorithm::SHA256:
			os << "SHA256";
			break;
		case HashAlgorithm::RIPEMD160:
			os << "RIPEMD160";
			break;
		default:
			os << "UNKNOWN HASH ALGORITHM";
		}

		return os;
	}

	HashFunction::HashFunction(HashAlgorithm hash_algo)
		: hash_func
	{ [hash_algo = std::move(hash_algo)](HashFunction::input_type input)
	{
		auto context = cactuar::HashContextPool::GetContext(hash_algo);

		return cactuar::HashDataWithContext(context, input);
	}
	}
	{}

	HashFunction::HashFunction(HashFunction::hash_function hash_func) 
		: hash_func{ hash_func } 
	{}

	HashFunction operator*(HashFunction&& lhs, HashFunction&& rhs)
	{
		return HashFunction(
			[g = std::move(lhs), f = std::move(rhs)]
		(const std::span<const std::byte> data)// mutable <- do you have any idea how long it took to find out
			{
				std::vector<std::byte> inner = f(data);
				return g({ inner.data(), inner.size() });
			}
		);
	}


	int HashSize(HashAlgorithm hash_algo)
	{
		auto context = cactuar::HashContextPool::GetContext(hash_algo);
		return cactuar::HashSize(context);
	}

} //namespace pulse