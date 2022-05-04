#ifndef __ETRO_FEDERATED_BLOCKCHAIN_H__
#define __ETRO_FEDERATED_BLOCKCHAIN_H__

#define FEDERATED_BLOCKCHAIN_UNROLLED_SIZE 32

/*********************************
* Federated block's header should be specified first so uses if extern etc here.
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <etro/federatedblock.h>

#ifdef __cplusplus
}
#endif


/*********************************
* Federated blockchain types. Specialised template class in c++ and
* undefined struct in C so as to be somewhat type safier than void *.
*/
#ifdef __cplusplus
#include <etro/blockchain.h>
typedef Blockchain<federatedblock_t, FEDERATED_BLOCKCHAIN_UNROLLED_SIZE> federated_blockchain_t;
#else
typedef struct federated_blockchain federated_blockchain_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

federated_blockchain_t* new_federated_blockchain(federatedblock_t genisis);

//Add some constness
void add(federated_blockchain_t* const chain, federatedblock_t block);

bool validate(federated_blockchain_t* const chain);


#ifdef __cplusplus
}
#endif





#endif