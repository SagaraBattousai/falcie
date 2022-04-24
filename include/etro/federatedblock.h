#ifndef __ETRO_FEDERATEDBLOCK_H__
#define __ETRO_FEDERATEDBLOCK_H__

//All C code!
#include <stdint.h>
#define SHA256_BYTES 32

	typedef unsigned char byte_t;
	typedef uint32_t cactuar;

	typedef struct block_header
	{
		uint32_t version;
		int64_t timestamp;
		byte_t prev_hash[SHA256_BYTES];
		byte_t transaction_hash[SHA256_BYTES];
		cactuar target;
		uint32_t nonce;
	} blockheader_t;


#endif