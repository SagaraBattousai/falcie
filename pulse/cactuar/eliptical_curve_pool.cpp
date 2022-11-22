
//Openssl Higher-level api
#include <openssl/evp.h>

//For Signing
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>

#include <cactuar/cactuar-eliptical_curve_pool.h>
//#include <cactuar/internal/cactuar-openssl_wrapper.h>

namespace pulse
{
	namespace cactuar
	{
		EVP_PKEY* ElipticalCurveContextPool::GetContext_impl(ElipticalCurves eliptical_curve)
		{
			int curve_NID;

			switch (eliptical_curve)
			{
			case ElipticalCurves::SECP256K1:
				curve_NID = NID_secp256k1;
				break;
			default:
				return nullptr;
			}

			EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);

			if (pctx == NULL) return nullptr;

			if (!EVP_PKEY_paramgen_init(pctx)) return nullptr;

			if (!EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, curve_NID)) return nullptr;

			EVP_PKEY *params = NULL;

			if (!EVP_PKEY_generate(pctx, &params)) return nullptr;

			pctx = EVP_PKEY_CTX_new(params, NULL);

			if (!EVP_PKEY_keygen_init(pctx)) return nullptr;

			EVP_PKEY *ppkey = NULL; //ppkey == public private key

			if (!EVP_PKEY_generate(pctx, &ppkey))
			{
				return nullptr;
			}

			EVP_PKEY_CTX_free(pctx);
			EVP_PKEY_free(params);

			return ppkey;
		}
	} //namespace pulse::cactuar

} //namespace pulse