#ifndef __PULSE_CACTUAR_CRYPTO_H__
#define __PULSE_CACTUAR_CRYPTO_H__

//#include <stdint.h>

#include <pulse/cactuar.h>

/*
typedef struct sha256_hash {
	unsigned char hash[SHA256];
}sha256hash_t;

typedef struct ripemd160_hash {
	unsigned char size = RIPEMD160;
	unsigned char hash[RIPEMD160];
}ripemd160hash_t;
*/

/*
typedef struct hash
{
	int size;
	unsigned char *hash;
} hash_t;
*/
typedef struct hasher hasher_t;


hasher_t* get_default_sha256_hasher();

/** size_t required for compatability with OpenSSL */
inline void update_hash(hasher_t* hasher, const void* data, size_t count);

/** unsigned int required for compatability with OpenSSL */
inline void output_hash(hasher_t* hasher, unsigned char* dst, unsigned int* size);


//Potentiall make inline, combo of the two above sans the func calls.
/** size_t, unsigned int required for compatability with OpenSSL */
void hash_data(hasher_t* hasher, const void* src, size_t src_count,
	unsigned char* dst, unsigned int* dst_size);

void free_hasher(hasher_t* hasher);


#endif