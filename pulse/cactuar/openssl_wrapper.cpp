//module;

#include <vector>
#include <span>

#include <openssl/evp.h>
#include <openssl/provider.h>
#include <openssl/err.h>

//Only needed in the static case with modules, which we can't use at the moment :(
//#if _WIN32
//#include <openssl/applink.c>
//#endif

//module cactuar:openssl_wraper; //private module
#include <cactuar/internal/cactuar-openssl_wrapper.h>

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
			EVP_MD *evp_type;

			switch (hash_type)
			{
			case HashAlgorithm::SHA256:
				LoadDefaultProvider();
				evp_type = EVP_MD_fetch(NULL, "SHA256", NULL);
				break;
			case HashAlgorithm::RIPEMD160:
				LoadLegacyProvider();
				evp_type = EVP_MD_fetch(NULL, "RIPEMD160", "provider=legacy");
				break;
			default:
				EVP_MD_CTX_free(context);
				return nullptr;
			}

			if (evp_type == NULL)
			{
				EVP_MD_CTX_free(context);
				return nullptr;
			}

			EVP_DigestInit_ex2(context, evp_type, NULL);

			EVP_MD_free(evp_type);

			return context;
		}

		std::vector<std::byte> HashDataWithContext(EVP_MD_CTX *context, const std::span<const std::byte> input)
		{
			std::span<std::byte>::size_type hashSize = EVP_MD_CTX_size(context);
			std::vector<std::byte> hashed_data(hashSize);

			EVP_DigestInit_ex2(context, NULL, NULL);
			EVP_DigestUpdate(context, input.data(), input.size());
			EVP_DigestFinal_ex(context, reinterpret_cast<unsigned char *>(hashed_data.data()), NULL);

			return hashed_data;
		};

		int HashSize(EVP_MD_CTX *context)
		{
			return EVP_MD_CTX_size(context);
		}

	} //namespace pulse::cactuar
} //namespace pulse