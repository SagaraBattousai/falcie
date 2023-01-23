

#include <cactuar/cactuar-eliptical_curve_pool.h>
#include <cactuar/cactuar-hash_pool.h>
#include <cactuar/cactuar-signing.h>

namespace pulse
{
	SigningFunction::SigningFunction(ElipticalCurve curve) 
		: curve {curve}
		, hash_algo {HashAlgorithm::COUNT} //Closest equivilent to a null value
		, hash_before_signing {false}
	{	}

	SigningFunction::SigningFunction(ElipticalCurve curve, HashAlgorithm hash_algo)
		: curve {curve}
		, hash_algo {hash_algo}
		, hash_before_signing {true}
	{	}

	CryptographicFunction::return_type
		SigningFunction::operator()(CryptographicFunction::input_type input) const
	{
		/*
		EVP_MD_CTX *mdctx = EVP_MD_CTX_create();

		if (!mdctx)
		{
			return -7;
		}
		//EVP_PKEY_CTX_free(pctx);


		EVP_PKEY* ppkey = pulse::cactuar::ElipticalCurveContextPool::GetContext(pulse::ElipticalCurves::SECP256K1);


		EVP_DigestSignInit(mdctx, NULL, NULL, NULL, ppkey);
		EVP_DigestSignUpdate(mdctx, "Hi There", 9);

		size_t siglen;
		unsigned char *sig = NULL;

		EVP_DigestSignFinal(mdctx, NULL, &siglen);

		sig = (unsigned char *)OPENSSL_malloc(sizeof(unsigned char) * siglen);

		EVP_DigestSignFinal(mdctx, sig, &siglen);

		EVP_DigestVerifyInit(mdctx, NULL, NULL, NULL, ppkey);

		EVP_DigestVerifyUpdate(mdctx, "Hi Thera", 9);

		int x = EVP_DigestVerifyFinal(mdctx, sig, siglen);

		std::cout << x << " Which means!" << std::endl;
		*/
		return CryptographicFunction::return_type();

	}
}