module;

#include <openssl/evp.h>

export module cactuar:crypto;

import <vector>;
import <span>;


namespace pulse
{
	export enum class HashType : unsigned char { SHA256 = 32, RIPEMD160 = 20 };

	namespace cactuar
	{
		EVP_MD_CTX* GetHashContext(HashType hash_type);

	} //namespace pulse::cactuar
} //namespace pulse

export namespace pulse
{
	template <HashType hash_type>
	class Hasher
	{
	public:
		//Need to understand explicit!!
		//explicit 
		Hasher();
		~Hasher();

		const std::vector<unsigned char>& Hash(const std::span<unsigned char>& input);

		/*
		* void UpdateHash(const void *input_data, size_t count)
			{ EVP_DigestUpdate(context, input_data, count); }

			void OutputHash()
			{ EVP_DigestFinal_ex(context, data, NULL); }
		*/

	private:
		EVP_MD_CTX *context;
		std::vector<unsigned char> hashed_data;
	};
	
	template <HashType hash_type>
	Hasher<hash_type>::Hasher() 
		: context(cactuar::GetHashContext(hash_type))
		, hashed_data(std::vector(static_cast<std::vector::size_type>(hash_type)))
	{}

	template <HashType hash_type>
	Hasher<hash_type>::~Hasher()
	{
		EVP_MD_CTX_free(this->context);
	}

	// Won't inline for now as it would force a recompilation and not just a relink. 
	template <HashType hash_type>
	const std::vector<unsigned char>& Hasher<hash_type>::Hash(const std::span<unsigned char>& input)
	{
		EVP_DigestInit_ex2(this->context, NULL, NULL);
		EVP_DigestUpdate(this->context, input.data(), input.size());
		EVP_DigestFinal_ex(this->context, this->hashed_data.data(), NULL);
		return this->hashed_data;
	}

} //namespace pulse
