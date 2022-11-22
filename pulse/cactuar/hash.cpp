//module;

#include <utility>
//module cactuar:hash;
//Openssl Higher-level api
#include <openssl/evp.h>
//Only needed in the static case with modules, which we can't use at the moment :(
//#if _WIN32
//#include <openssl/applink.c>
//#endif

//For Hashing
#include <openssl/provider.h>
#include <openssl/err.h>


#include <cactuar/cactuar-hash.h>
#include <cactuar/cactuar-hash_pool.h>
//#include <cactuar/internal/cactuar-openssl_wrapper.h>

namespace pulse
{
	namespace cactuar
	{

		std::vector<std::byte> HashDataWithContext(EVP_MD_CTX *context, const std::span<const std::byte> input)
		{
			std::span<std::byte>::size_type hashSize = EVP_MD_CTX_size(context);
			std::vector<std::byte> hashed_data(hashSize);

			EVP_DigestInit_ex2(context, NULL, NULL);
			EVP_DigestUpdate(context, input.data(), input.size());
			EVP_DigestFinal_ex(context, reinterpret_cast<unsigned char *>(hashed_data.data()), NULL);

			return hashed_data;
		}

		int HashSize(EVP_MD_CTX *context)
		{
			return EVP_MD_CTX_size(context);
		}
	}

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
		//auto context = cactuar::HashContextPool::GetContext(hash_algo);
		auto context = cactuar::HashContextPool::GetContext(hash_algo);
		return cactuar::HashSize(context);
	}

} //namespace pulse