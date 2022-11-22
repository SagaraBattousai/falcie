#ifndef __CACTUAR_INTERNAL_OPENSSL_WRAPPER_H__
#define __CACTUAR_INTERNAL_OPENSSL_WRAPPER_H__

#include <vector>
#include <span>
#include <cstddef>

#include <cactuar/cactuar-crypto.h>

typedef struct evp_md_ctx_st EVP_MD_CTX; //forward declaration with a typedef

namespace pulse
{
	namespace cactuar
	{

		EVP_MD_CTX* GetHashContext(HashAlgorithm hash_type);

		std::vector<std::byte> HashDataWithContext(EVP_MD_CTX *context, const std::span<const std::byte> input);

		int HashSize(EVP_MD_CTX *context); //cant be inline (given headers) but smart compilers will save us

	}
}

#endif // !__CACTUAR_INTERNAL_OPENSSL_WRAPPER_H__
