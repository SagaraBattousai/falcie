#ifndef __ETRO_FEDERATEDBLOCK_H__
#define __ETRO_FEDERATEDBLOCK_H__

#include <stdint.h>
#include <pulse/types.h>
#include <pulse/cactuar.h>

typedef struct network_update
{
	int64_t num_layers;
	int64_t *network_dims; ///< Must have @c num_layer @c + @c 1 many elements.
						   ///< i.e. input_layer_size + neuron_dims
	float **delta_weights;
	int64_t examples_seen;

}network_update_t;

typedef struct federated_block
{
	uint32_t magic; //Always 0x43616C6F
	uint32_t blocksize; ///< same as bitcoin, although unsinged aren't liked
						///< it's the right choice for here I think.
	blockheader_t *header;
	int64_t local_update_count; ///< actual number of local updates assigned.
	int64_t local_update_size; ///< size of malloced local updates (first dimension).
	network_update_t **local_updates;
	network_update_t *global_update;

	//varint Transaction counter
	//transactions	the(non empty) list of transactions
	//TODO: Just for now VV
	//LocalUpdates[]cactuar.Pair [[]float32, int]
	//	GlobalUpdate[]float32

} federatedblock_t;

federatedblock_t new_federated_block();

unsigned char *hash_federated(const federatedblock_t* const block);

void mine_federated(federatedblock_t* const block, const unsigned char* const prev_hash);

unsigned char *prev_hash_federated(const federatedblock_t* const block);

cactuar_t target_federated(const federatedblock_t* const block);

network_update_t *new_global_update(int64_t num_layers, int64_t *network_dims); ///<Should probably be hidden;

int add_local_update(federatedblock_t *block, network_update_t *update);

void get_global_update(federatedblock_t *block, network_update_t **update);

int calculateGlobalUpdate(federatedblock_t *block);


#endif