#ifndef __ETRO_TYPES_H__
#define __ETRO_TYPES_H__

#include <stdint.h>

#define SHA256_BYTES 32

typedef unsigned char byte_t;
typedef uint32_t cactuar;

typedef struct sha256_hash {
	byte_t hash[SHA256_BYTES];
}sha256hash_t;

#endif