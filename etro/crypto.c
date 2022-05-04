#include <openssl/evp.h>
#include <etro/crypto.h>

//Not thread safe
EVP_MD_CTX* get_default_sha256_context()
{
	static EVP_MD_CTX *sha256_ctx = NULL;
	if (sha256_ctx == NULL)
	{
		sha256_ctx = EVP_MD_CTX_new();
		EVP_DigestInit_ex2(sha256_ctx, EVP_sha256(), NULL);
	}
	//TODO: Since first cond will only happen on first call is it better to not use else and 
	//just double init on first instansiation or add extra stack member (type)
	else 
	{
		EVP_DigestInit_ex2(sha256_ctx, NULL, NULL);
	}

	return sha256_ctx;
}


/*
mdctx = EVP_MD_CTX_new();
EVP_DigestInit_ex2(mdctx, md, NULL);
EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
EVP_DigestFinal_ex(mdctx, md_value, &md_len);
EVP_MD_CTX_free(mdctx);
*/