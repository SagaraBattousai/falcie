#ifndef __ETRO_FEDERATEDBLOCK_H__
#define __ETRO_FEDERATEDBLOCK_H__

#include <stdint.h>
#include <pulse/types.h>
#include <pulse/blockheader.h>
#include <pulse/cactuar.h>

typedef struct federated_block
{
	blockheader_t header;
} federatedblock_t;

federatedblock_t new_federated_block();

sha256hash_t hash_federated(const federatedblock_t* const block);

void mine_federated(federatedblock_t* const block, const sha256hash_t* const prev_hash);

sha256hash_t prev_hash_federated(const federatedblock_t* const block);

cactuar_t target_federated(const federatedblock_t* const block);


#endif