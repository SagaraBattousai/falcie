module;

#include <cstdint>

export module orphan:federated_block;

import <vector>;
import <memory>;

import pulse;

import :federated_network;

class Federatedblock
{
public:

	Federatedblock();

	unsigned char *hash_federated(const federatedblock_t* const block);

	void mine_federated(federatedblock_t* const block, const unsigned char* const prev_hash);

	unsigned char *prev_hash_federated(const federatedblock_t* const block);

	cactuar_t target_federated(const federatedblock_t* const block);

	network_update_t *new_global_update(int64_t num_layers, int64_t *network_dims); ///<Should probably be hidden;

	int add_local_update(federatedblock_t *block, network_update_t *update);

	void get_global_update(federatedblock_t *block, network_update_t **update);

	int calculateGlobalUpdate(federatedblock_t *block);



private:
	std::uint32_t magic; //Always 0x43616C6F
	std::uint32_t blocksize;
	pulse::Blockheader header;
	std::vector<NetworkUpdate> local_updates;
	std::unique_ptr<NetworkUpdate> global_update;

};