
#include <pulse/federatedblock.h>
#include <pulse/blockheader.h>
#include <pulse/cactuar.h>

/*
*void mine(blockheader_t * const header);

sha256hash_t hash(const blockheader_t* const header);*/

federatedblock_t new_federated_block()
{
	//nonce ~must/should be 0 and target will be set for now but can change when needed
	blockheader_t header = { .version = 0x01, .nonce = 0, .target = CACTUAR_BASE_DIFFICULTY };

	//TODO: fix
	federatedblock_t block = { 0 }; // { header };

	return block;
}


sha256hash_t hash_federated(const federatedblock_t* const block)
{
	return hash(&block->header);
}

void mine_federated(federatedblock_t* const block, const sha256hash_t* const prev_hash)
{
	mine(&(block->header), prev_hash);
}

sha256hash_t prev_hash_federated(const federatedblock_t* const block)
{
	return block->header.prev_hash;
}

cactuar_t target_federated(const federatedblock_t* const block)
{
	return block->header.target;
}
