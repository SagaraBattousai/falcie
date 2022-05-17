#ifndef __LINDZEI_ORPHAN_FEDERATED_BLOCKCHAIN_H__
#define __LINDZEI_ORPHAN_FEDERATED_BLOCKCHAIN_H__

/*********************************
* Federated block's header should be specified first so uses if extern etc here.
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <orphan/federatedblock.h>

#ifdef __cplusplus
}
#endif


/*********************************
* Federated blockchain types. Specialised template class in c++ and
* undefined struct in C so as to be somewhat type safier than void *.
*/
#ifdef __cplusplus
#include <cstdint>
#include <pulse/dahaka.h>

constexpr int64_t FEDERATED_BLOCKCHAIN_UNROLLED_SIZE = 32;

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