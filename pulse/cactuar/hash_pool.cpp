
//Openssl Higher-level api
#include <openssl/evp.h>

//For Hashing
#include <openssl/provider.h>
#include <openssl/err.h>

#include <cactuar/cactuar-hash_pool.h>

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

		EVP_MD* HashContextPool::GetDigestType(HashAlgorithm hash_algo)
		{
			switch (hash_algo)
			{
			case HashAlgorithm::SHA256:
				LoadDefaultProvider();
				return EVP_MD_fetch(NULL, "SHA256", NULL);
				//break;
			case HashAlgorithm::RIPEMD160:
				LoadLegacyProvider();
				return EVP_MD_fetch(NULL, "RIPEMD160", "provider=legacy");
				//break;
			default:
				return nullptr;
			}
		}


		EVP_MD_CTX* HashContextPool::GetContext_impl(HashAlgorithm hash_algo)
		{

			EVP_MD *evp_type = GetDigestType(hash_algo); // or const &

			if (evp_type == nullptr)
			{
				return nullptr;
			}

			EVP_MD_CTX *context = EVP_MD_CTX_new();

			EVP_DigestInit_ex2(context, evp_type, NULL);

			EVP_MD_free(evp_type);

			return context;

		}
	} //namespace pulse::cactuar

} //namespace pulse