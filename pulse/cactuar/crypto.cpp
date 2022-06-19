//module;

#include <openssl/evp.h>
#include <openssl/provider.h>
#include <openssl/err.h>

//Only needed in the static case with modules, which we can't use at the moment :(
//#if _WIN32
//#include <openssl/applink.c>
//#endif

//module cactuar:crypto;
#include <cactuar/cactuar-crypto.h>


namespace pulse
{
	namespace cactuar
	{
		namespace
		{
			OSSL_PROVIDER *OSSL_LEGACY_PROVIDER = nullptr;
			OSSL_PROVIDER *OSSL_DEFAULT_PROVIDER = nullptr;

			inline void LoadDefaultProvider()
			{
				if (OSSL_DEFAULT_PROVIDER == nullptr)
					OSSL_DEFAULT_PROVIDER = OSSL_PROVIDER_load(NULL, "default");
			}
			inline void LoadLegacyProvider()
			{
				if (OSSL_LEGACY_PROVIDER == NULL)
					OSSL_LEGACY_PROVIDER = OSSL_PROVIDER_load(NULL, "legacy");
			}
		}

		EVP_MD_CTX* GetHashContext(HashAlgorithm hash_type)
		{
			EVP_MD_CTX *context = EVP_MD_CTX_new();
			const EVP_MD *evp_type; //No one's told me if i need to delete this or not ....

			switch (hash_type)
			{
			case HashAlgorithm::SHA256:
				LoadDefaultProvider();
				evp_type = EVP_sha256();
				break;
			case HashAlgorithm::RIPEMD160:
				LoadLegacyProvider();
				evp_type = EVP_ripemd160();
				//EVP_MD_fetch(NULL, "RIPEMD160", "provider=legacy");; 
				break;
			default:
				EVP_MD_CTX_free(context);
				return nullptr;
			}

			EVP_DigestInit_ex2(context, evp_type, NULL);

			return context;
		}

	} //namespace pulse::cactuar

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
		: context(cactuar::GetHashContext(hash_algo))
	{}

	HashFunction::~HashFunction() { EVP_MD_CTX_free(this->context); }

	HashFunction::hash_type HashFunction::operator()(const std::span<const std::byte> input)
	{
		std::span<std::byte>::size_type hashSize = EVP_MD_CTX_size(this->context);
		std::vector<std::byte> hashed_data(hashSize);

		EVP_DigestInit_ex2(this->context, NULL, NULL);
		EVP_DigestUpdate(this->context, input.data(), input.size());
		EVP_DigestFinal_ex(this->context, reinterpret_cast<unsigned char *>(hashed_data.data()), NULL);

		return hashed_data;
	};

	/*
		* void UpdateHash(const void *input_data, size_t count)
			{ EVP_DigestUpdate(context, input_data, count); }

			void OutputHash()
			{ EVP_DigestFinal_ex(context, data, NULL); }
		*/



} //namespace pulse