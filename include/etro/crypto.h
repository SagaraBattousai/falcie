#ifndef __ETRO_CRYPTO_H__
#define __ETRO_CRYPTO_H__
//All C code!
#include <etro/types.h>

typedef struct hasher hasher_t;

hasher_t* get_default_sha256_hasher();

inline void update_hash(hasher_t* hasher, const void* data, size_t count);

inline void output_hash(hasher_t* hasher, unsigned char* dst, unsigned int* size);

//Potentiall make inline, combo of the two above sans the func calls.
void hash_data(hasher_t* hasher, const void* src, size_t src_count,
	unsigned char* dst, unsigned int* dst_size);

void free_hasher(hasher_t* hasher);


#endif