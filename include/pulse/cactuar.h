#ifndef __PULSE_CACTUAR_H__
#define __PULSE_CACTUAR_H__

/*************************************************************
************** Cactuar's Public API Header *******************
**************************************************************/

#include <stdint.h>

/// Value is the size of the type in bytes
enum hash_type_t {SHA256 = 32,  RIPEMD160 = 20};

typedef uint32_t cactuar_t;

//typedef struct hash hash_t;
//TODO: Remove, only for debugging!
//That said, may want to expose as its somewhat useful ....
typedef struct hash
{
    int size;
    unsigned char *hash;
} hash_t;

//typedef struct blockheader blockheader_t;
//TODO: Remove, only for debugging!
//That said, may want to expose as its somewhat useful ....
typedef struct blockheader
{
    uint32_t version;
    int64_t timestamp;
    hash_t *prev_hash;
    hash_t *transaction_hash;
    uint32_t target;
    uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
} blockheader_t;

blockheader_t *new_blockheader(uint32_t version);

hash_t *get_prev_hash(blockheader_t *header);

cactuar_t *get_target(blockheader_t *header);

int targetcmp(const cactuar_t* const cactuar, const unsigned char* const hash);

void mine(blockheader_t * const header, const unsigned char* const prev_hash);

hash_t *hash(const blockheader_t* const header);

int64_t generate_timestamp();

//extern inline cactuar_t base_difficulty();

void free_hash(hash_t *hash);



#endif