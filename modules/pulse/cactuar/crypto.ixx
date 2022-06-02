module;

#include <openssl/evp.h>
#include <cstddef>

export module cactuar:crypto;

import <vector>;
import <algorithm>;
import <span>;
import <memory>;
import <map>;

export namespace pulse
{
	enum class HashAlgorithm : unsigned char { SHA256 = 32, RIPEMD160 = 20 };

	//constexpr funcs are implicitly inline
	constexpr unsigned char AsHashSize(HashAlgorithm hash_algo) 
	{ return static_cast<unsigned char>(hash_algo); };

} // namespace pulse

namespace pulse
{
	namespace cactuar
	{
		EVP_MD_CTX* GetHashContext(HashAlgorithm hash_algo);

		void BaseHashFunction(EVP_MD_CTX*, const std::span<std::byte>&, std::span<std::byte>);

		void BaseHashFunction(EVP_MD_CTX *context, const std::span<std::byte>& input, std::span<std::byte> output)
		{
			EVP_DigestInit_ex2(context, NULL, NULL);
			EVP_DigestUpdate(context, input.data(), input.size());
			EVP_DigestFinal_ex(context, reinterpret_cast<unsigned char *>(output.data()), NULL);
		}
	} //namespace pulse::cactuar
} //namespace pulse

export namespace pulse
{
	//Could make singleton and then use mutex's
	template <HashAlgorithm hash_algo>
	class HashFunction
	{
	public:
		HashFunction();
		~HashFunction();

		using hash_type = std::array<std::byte, AsHashSize(hash_algo)>;

		inline const hash_type operator()(const std::span<std::byte>& input) const
		{
			hash_type hashed_data{};
			cactuar::BaseHashFunction(this->context, input, { hashed_data.data(), hashed_data.size() });
			return hashed_data;
		};

		/*
		* void UpdateHash(const void *input_data, size_t count)
			{ EVP_DigestUpdate(context, input_data, count); }

			void OutputHash()
			{ EVP_DigestFinal_ex(context, data, NULL); }
		*/

	private:
		EVP_MD_CTX *context;
		//hash_type hashed_data;
	};
	
	template <HashAlgorithm hash_algo>
	HashFunction<hash_algo>::HashFunction() : context(cactuar::GetHashContext(hash_algo)) {}

	template <HashAlgorithm hash_algo>
	HashFunction<hash_algo>::~HashFunction()
	{
		EVP_MD_CTX_free(this->context);
	}


	//TODO: make concurrent and make a true pool using things such as "in_use" and locks/seamaphores
	template <HashAlgorithm hash_algo>
	class HashFunctionPool
	{
	public:
		static const HashFunction<hash_algo>& Instance()
		{
			if (instance.get() == nullptr)
			{
				instance.reset(new HashFunctionPool<hash_algo>());
			}
			return (instance->pool)[0];
		};

	private:

		//Why is the array elems being destructed!!! because you werent passing by ref but by val
		// Still unsure why that causes deletion as I would expect it to just delete the copy.
		// I think its because array[] returns a ref
		// 
		//TODO: Should be auto initialized, else I'll switch to veccy
		//make more than 1 once seamaphores implmented.
		std::array<HashFunction<hash_algo>, 1> pool;	

		static inline std::unique_ptr<HashFunctionPool<hash_algo>> instance = 
			std::unique_ptr<HashFunctionPool>{ nullptr };

	};

} //namespace pulse
