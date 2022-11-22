#ifndef __CACTUAR_HASH_POOL_H__
#define __CACTUAR_HASH_POOL_H__

#include <vector>
#include <array>
#include <memory>

#include <cactuar/cactuar-crypto.h>

typedef struct evp_md_ctx_st EVP_MD_CTX;

namespace pulse
{
		namespace cactuar
	{
		//TODO: make concurrent and make a true pool using things such as "in_use" and locks/seamaphores
		class HashContextPool
		{
		public:
			static HashContextPool& Instance();

			//Don't want to expose anything EVP therefore the Pool is now in cactuar and therefore hidden
			static EVP_MD_CTX* GetContext(HashAlgorithm);

		private:

			//HashContextPool(); No need as only static member variables exist

			//Why not make it static since ... There can only be one!
			static std::vector<std::array<EVP_MD_CTX*, 1>>pool;

			//inline as multiple definitions (but all are the same and all are in different translation units) (C++17)
			static inline std::unique_ptr<HashContextPool> instance =
				std::unique_ptr<HashContextPool>{ nullptr };
		};

	} //namespace pulse::cactuar 
} //namespace pulse


#endif // !__CACTUAR_HASH_POOL_H__
