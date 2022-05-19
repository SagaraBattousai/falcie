#ifndef __ETRO_BLOCKHEADER_H__
#define __ETRO_BLOCKHEADER_H__

#include <stdint.h>

#include <pulse/cactuar.h>
#include <cactuar/crypto.h>

/*******************************************************************
* The header for  block on the chain which is hashed to provide PoW
********************************************************************/
/*
typedef struct blockheader
{
    uint32_t version;
    int64_t timestamp;
    hash_t *prev_hash;
    hash_t *transaction_hash;
    uint32_t target;
    uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
} blockheader_t;
*/


hash_t* hash(const blockheader_t* const header);

//int64_t generate_timestamp();


#endif