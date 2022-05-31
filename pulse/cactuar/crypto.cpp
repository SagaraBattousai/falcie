module;

#include <openssl/evp.h>
#include <openssl/provider.h>
#include <openssl/err.h>

// Sadly due to (I'm guessing) modules and staticness of library
// Linking to OpenSSL::applink doesn't work. Therefore, it is #included
// here and promises NOT to be included anywhere else!
#if _WIN32
#include <openssl/applink.c>
#endif

module cactuar:crypto;

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


		EVP_MD_CTX* GetHashContext(HashType hash_type)
		{
			EVP_MD_CTX *context = EVP_MD_CTX_new();
			const EVP_MD *evp_type; //No one's told me if i need to delete this or not ....

			switch (hash_type)
			{
			case HashType::SHA256:
				LoadDefaultProvider();
				evp_type = EVP_sha256();
				break;
			case HashType::RIPEMD160:
				LoadLegacyProvider();
				evp_type = EVP_MD_fetch(NULL, "RIPEMD160", "provider=legacy");; //EVP_ripemd160();
				break;
			default:
				EVP_MD_CTX_free(context);
				return nullptr;
			}

			EVP_DigestInit_ex2(context, evp_type, NULL);

			return context;
		}


	} //namespace pulse::cactuar
} //namespace pulse