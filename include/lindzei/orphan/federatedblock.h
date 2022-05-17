#ifndef __ETRO_FEDERATEDBLOCK_H__
#define __ETRO_FEDERATEDBLOCK_H__

#include <stdint.h>
#include <pulse/types.h>
#include <pulse/cactuar.h>

/************ JUST FOR NOW *************************/
/*******************************************************************
* The header for  block on the chain which is hashed to provide PoW
********************************************************************/

typedef struct block_header blockheader_t;

void mine(blockheader_t * const header, const sha256hash_t* const prev_hash);

sha256hash_t hash(const blockheader_t* const header);

uint64_t generate_timestamp();

/********************************************************************/

typedef struct federated_block
{
	unsigned int magic; //Always 0x43616C6F
	unsigned int blocksize;
	blockheader_t *header;
	//varint Transaction counter
	//transactions	the(non empty) list of transactions
	//TODO: Just for now VV
	//LocalUpdates[]cactuar.Pair [[]float32, int]
	//	GlobalUpdate[]float32

} federatedblock_t;

federatedblock_t new_federated_block();

sha256hash_t hash_federated(const federatedblock_t* const block);

void mine_federated(federatedblock_t* const block, const sha256hash_t* const prev_hash);

sha256hash_t prev_hash_federated(const federatedblock_t* const block);

cactuar_t target_federated(const federatedblock_t* const block);


#endif