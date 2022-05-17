#ifndef __ETRO_BLOCKHEADER_H__
#define __ETRO_BLOCKHEADER_H__

#include <stdint.h>

#include <cactuar/crypto.h>

/*******************************************************************
* The header for  block on the chain which is hashed to provide PoW
********************************************************************/

typedef struct block_header
{
    uint32_t version;
    //unsigned as time can't be from before the epoch!
    //Also allows us to use 0 as an error cond
    uint64_t timestamp;
    sha256hash_t prev_hash;
    sha256hash_t transaction_hash;
    uint32_t target;
    uint32_t nonce;
} blockheader_t;

void mine(blockheader_t * const header, const sha256hash_t* const prev_hash);

sha256hash_t hash(const blockheader_t* const header);

uint64_t generate_timestamp();


#endif