#ifndef __CACTUAR_HASH_POOL_H__
#define __CACTUAR_HASH_POOL_H__

#include <vector>
#include <array>
#include <memory>

#include <cactuar/cactuar-crypto.h>
#include <cactuar/cactuar-crypto_pool.h>

typedef struct evp_pkey_st EVP_PKEY;

namespace pulse
{
	namespace cactuar
	{
		class ElipticalCurveContextPool : public ContextPool<ElipticalCurveContextPool, EVP_PKEY, ElipticalCurves>
		{
		private:
			virtual EVP_PKEY* GetContext_impl(ElipticalCurves) override;
		};

	} //namespace pulse::cactuar 
} //namespace pulse


#endif // !__CACTUAR_HASH_POOL_H__
