
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>

#include <iostream>
#include <vector>
#include <cstddef>


#include <pulse/cactuar/cactuar-hash.h>

int main()
{
	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);

	if (pctx == NULL)
	{
		return -1;
	}

	if (!EVP_PKEY_paramgen_init(pctx))
	{
		return -2;
	}

	if (!EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_secp256k1))
	{
		return -3;
	}

	EVP_PKEY *params = NULL;

	if (!EVP_PKEY_generate(pctx, &params))
	{
		return -4;
	}

	pctx = EVP_PKEY_CTX_new(params, NULL);

	if (!EVP_PKEY_keygen_init(pctx))
	{
		return -5;
	}

	EVP_PKEY *ppkey = NULL; //ppkey == public private key

	if (!EVP_PKEY_generate(pctx, &ppkey))
	{
		return -6;
	}

	//int EVP_DigestSignInit(EVP_MD_CTX *ctx, EVP_PKEY_CTX **pctx, const EVP_MD *type, ENGINE *e, EVP_PKEY *pkey);

	EVP_MD_CTX *mdctx = EVP_MD_CTX_create();

	if (!mdctx)
	{
		return -7;
	}


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


	pulse::HashFunction sha256 = pulse::HashFunction(pulse::HashAlgorithm::SHA256);
	std::vector<std::byte> data = { std::byte{0x01}, std::byte{0x02}, std::byte{0x03}, std::byte{0x04}, std::byte{0x05} };
	auto hash = sha256(data);
	hash = sha256(hash);

	auto dhf = pulse::HashFunction(pulse::HashAlgorithm::SHA256) * pulse::HashFunction(pulse::HashAlgorithm::SHA256);

	auto double_hash = dhf(data);

	for (auto d : hash)
	{
		std::cout << std::hex << static_cast<int>(d);
	}

	std::cout << std::endl;

	for (auto d : double_hash)
	{
		std::cout << std::hex << static_cast<int>(d);
	}

	std::cout << std::endl;


	return 0;
}