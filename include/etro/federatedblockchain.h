#ifndef __ETRO_FEDERATED_BLOCKCHAIN_H__
#define __ETRO_FEDERATED_BLOCKCHAIN_H__

#ifndef BLOCKCHAIN_CHAIN_SIZE
#define BLOCKCHAIN_CHAIN_SIZE 32
#endif

#ifdef __cplusplus
#include <cstdint>
#include <dahaka/dahaka-blockchain.h>
#include <orphan/federated_blockchain.h>
#else
#include <stdint.h>
#endif

#include <etro/federatedblock.h>
#include <etro/networkupdate.h>

//Typedefs for C vs CPP
#ifdef __cplusplus
typedef pulse::Blockchain<lindzei::Federatedblock, BLOCKCHAIN_CHAIN_SIZE> federated_blockchain_t;
#else
typedef struct federated_blockchain federated_blockchain_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif
	federated_blockchain_t* new_federated_blockchain(federated_block_builder_t *genisis_builder, const int hash_size);

	void print_blockchain(federated_blockchain_t *chain);

	int add_block_to_chain(federated_blockchain_t *chain, federated_block_t *block);

	int get_last_added_block(federated_blockchain_t *chain, const federated_block_t **block);
	
	void delete_federated_blockchain(federated_blockchain_t *update);

#ifdef __cplusplus
}
#endif



#endif