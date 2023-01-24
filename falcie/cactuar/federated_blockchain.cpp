/*
#include <orphan/federated_blockchain.h>
#include <pulse/dahaka.h>

extern "C" {
#include <orphan/federatedblock.h>
}


federated_blockchain_t* new_federated_blockchain(federatedblock_t genisis)
{
	return new federated_blockchain_t(
		genisis,
		&mine_federated,
		&hash_federated,
		&prev_hash_federated,
		&target_federated,
		&targetcmp,
		SHA256);
}

void add(federated_blockchain_t* const chain, federatedblock_t block)
{
	chain->Add(block);
}

federatedblock_t get_last(federated_blockchain_t* const chain)
{
	return chain->GetLast();
}


bool validate(federated_blockchain_t* const chain)
{
	return chain->Validate();
}

*/