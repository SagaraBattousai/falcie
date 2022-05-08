
#include <pulse/federated_blockchain.h>
#include <pulse/blockchain.h>

extern "C" {
#include <pulse/federatedblock.h>
}


federated_blockchain_t* new_federated_blockchain(federatedblock_t genisis)
{
	return new federated_blockchain_t(
		genisis,
		&mine_federated,
		&hash_federated,
		&prev_hash_federated,
		&target_federated);
}

void add(federated_blockchain_t* const chain, federatedblock_t block)
{
	chain->Add(block);
}

bool validate(federated_blockchain_t* const chain)
{
	return chain->Validate();
}
