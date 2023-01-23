#ifndef __CACTUAR_HASH_POOL_H__
#define __CACTUAR_HASH_POOL_H__

#include <vector>
#include <array>
#include <memory>

#include <cactuar/cactuar-crypto.h>
#include <cactuar/cactuar-crypto_pool.h>

typedef struct evp_md_ctx_st EVP_MD_CTX;
typedef struct evp_md_st EVP_MD;

namespace pulse
{
	namespace cactuar
	{
		class HashContextPool : public ContextPool<HashContextPool, EVP_MD_CTX, HashAlgorithm>
		{
		public:
			static EVP_MD* GetDigestType(HashAlgorithm hash_algo);
		private:
			virtual EVP_MD_CTX* GetContext_impl(HashAlgorithm hash_algo) override;
		};

	} //namespace pulse::cactuar 
} //namespace pulse


#endif // !__CACTUAR_HASH_POOL_H__
