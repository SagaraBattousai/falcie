#ifndef __ETRO_FEDERATEDBLOCK_H__
#define __ETRO_FEDERATEDBLOCK_H__

#include <stdint.h>
#include <etro/blockheader.h>
#define SHA256_BYTES 32

	typedef unsigned char byte_t;
	typedef uint32_t cactuar;

	typedef struct federated_block
	{
		blockheader_t* header;
	} federatedblock_t;

sha256hash_t hash(const federatedblock_t* const header);

void mine(const federatedblock_t* const header);


#endif