#ifndef __ETRO_CRYPTO_H__
#define __ETRO_CRYPTO_H__
//All C code!
#include <openssl/evp.h>
#include <etro/types.h>

EVP_MD_CTX* get_default_sha256_context();



#endif