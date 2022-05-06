#include <stdlib.h>

#include <openssl/evp.h>
#include <etro/crypto.h>

typedef struct hasher 
{
	EVP_MD_CTX* hash_context;
}hasher_t;


//Not thread safe
hasher_t* get_default_sha256_hasher()
{
	static hasher_t* sha256 = NULL;
	if (sha256 == NULL)
	{
		sha256 = (hasher_t*)malloc(sizeof(struct hasher));
		if (sha256 == NULL)
		{
			return NULL;
		}
			
		sha256->hash_context = EVP_MD_CTX_new();
		EVP_DigestInit_ex2(sha256->hash_context, EVP_sha256(), NULL);
	}
	//TODO: Since first cond will only happen on first call is it better to not use else and 
	//just double init on first instansiation or add extra stack member (type)
	else 
	{
		EVP_DigestInit_ex2(sha256->hash_context, NULL, NULL);
	}

	return sha256;
}

void update_hash(hasher_t* hasher, const void *data, size_t count)
{
	EVP_DigestUpdate(hasher->hash_context, data, count);
}

void output_hash(hasher_t* hasher, unsigned char *dst, unsigned int *size)
{
	EVP_DigestFinal_ex(hasher->hash_context, dst, size);
}

void hash_data(hasher_t* hasher, const void* src, size_t src_count,
	unsigned char* dst, unsigned int* dst_size)
{
	EVP_DigestUpdate(hasher->hash_context, src, src_count);
	EVP_DigestFinal_ex(hasher->hash_context, dst, dst_size);
}

void free_hasher(hasher_t *hasher)
{
	EVP_MD_CTX_free(hasher->hash_context);
	free(hasher);
	hasher = NULL;
}


/*
mdctx = EVP_MD_CTX_new();
EVP_DigestInit_ex2(mdctx, md, NULL);
EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
EVP_DigestFinal_ex(mdctx, md_value, &md_len);
EVP_MD_CTX_free(mdctx);
*/