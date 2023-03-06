#ifndef __ETRO_BLOCKCHAIN_H__
#define __ETRO_BLOCKCHAIN_H__

#ifndef BLOCKCHAIN_CHAIN_SIZE
#define BLOCKCHAIN_CHAIN_SIZE 32
#endif

#ifdef __cplusplus
#include <cstdint>
#include <cactuar/cactuar-blockchain.h>
#else
#include <stdint.h>
#endif

#include <etro/block.h>
#include <etro/networkupdate.h>

//Typedefs for C vs CPP
#ifdef __cplusplus
typedef cactuar::Blockchain blockchain_t;
#else
typedef struct blockchain blockchain_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif
	blockchain_t* new_blockchain();

	void print_blockchain(blockchain_t *chain);

	int add_block_to_chain(blockchain_t *chain, block_t *block);

	int get_last_added_block(blockchain_t *chain, const block_t **block);
	
	void delete_blockchain(blockchain_t *chain);

#ifdef __cplusplus
}
#endif



#endif