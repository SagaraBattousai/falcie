/* UNused Now (I think)

module;

#include <cstdint>

export module orphan:federated_blockchain;

export namespace pulse
{

constexpr int64_t FEDERATED_BLOCKCHAIN_UNROLLED_SIZE = 32;

typedef Blockchain<federatedblock_t, unsigned char, FEDERATED_BLOCKCHAIN_UNROLLED_SIZE> federated_blockchain_t;
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

	federatedblock_t get_last(federated_blockchain_t* const chain);



}
*/