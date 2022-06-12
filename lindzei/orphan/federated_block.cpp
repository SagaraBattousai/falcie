
#include <vector> //import <vector>;


#include <pulse/pulse.h> //import pulse;
#include <cactuar/cactuar-target.h>

#include <orphan/federated_block.h>

//export
namespace lindzei
{
	Federatedblock::Federatedblock(std::uint32_t version, pulse::Target target,
		BlockHashFunction hash_func, pulse::HashAlgorithm hash_algo)
		: Federatedblock(pulse::Blockheader{
			version,
			-1,
			std::vector<std::byte>{AsHashSize(hash_algo)},
			std::vector<std::byte>{AsHashSize(hash_algo)},
			target,
			0
			}, hash_func, hash_algo)
	{
	}

	Federatedblock::Federatedblock(pulse::Blockheader&& header,
		BlockHashFunction hash_func, pulse::HashAlgorithm hash_algo)
		: magic{ 0x43616C6F }
		//, header{ std::move(header) }
		//, header{ std::make_unique<pulse::Blockheader>(std::move(header)) }
		, header{ std::make_shared<pulse::Blockheader>(std::move(header)) }
		, local_updates{ std::vector<NetworkUpdate>{} }
		, global_update{ }// std::make_unique<NetworkUpdate>() }
		, hash_algo{ hash_algo }
		, hash_func{ hash_func }
	{}

	Federatedblock Federatedblock::Genisis(std::uint32_t version,
		BlockHashFunction hash_func, pulse::HashAlgorithm hash_algo)
	{
		return Federatedblock(
			pulse::Blockheader::Genisis(hash_algo, version), hash_func, hash_algo);
	}

	void Federatedblock::Mine(blockhash_type&& prev_hash)
	{
		this->header->prev_hash = std::move(prev_hash);
		this->header->timestamp = Block::GenerateTimestamp();

		//TODO: add error for invalid cactaur values
		std::vector<std::byte> difficulty_array = this->header->target.Expand();

		blockhash_type currHash = this->Hash();

		while (currHash > difficulty_array)
		{
			this->header->nonce += 1;
			currHash = this->Hash();
		}

		CalculateGlobalUpdate();

	}

	//I think this can be rvalue since we dont need it after its added.
	void Federatedblock::AddLocalUpdate(NetworkUpdate&& update)
	{
		//TODO: For now, probably init block with network stucture (and possibly builder)
		//To enfores all updates to be of a compatible network
		//may need to change to make network_structure part of the block constructor
		if (this->global_update.network_structure.size() == 0)
		{
			this->global_update = GlobalNetworkUpdate(update.network_structure);
		}

		this->local_updates.push_back(std::move(update));
	}


	inline const bool Federatedblock::CompareWithTarget(const Federatedblock::blockhash_type& hash) const
	{
		return hash < this->header.get()->target;
	}

	inline std::vector<std::byte> Federatedblock::State() const
	{
		return pulse::BlockheaderState(this->header.get(), this->hash_algo);
	}

	const NetworkUpdate& Federatedblock::GetGlobalUpdate() const
	{
		return this->global_update;
	}

	void Federatedblock::CalculateGlobalUpdate()
	{
		for (const NetworkUpdate& update : this->local_updates)
		{
			this->global_update.examples_seen += update.examples_seen;

			for (std::int64_t i = 0;
				i < static_cast<std::int64_t>(this->global_update.network_structure.size()) - 1;
				i++)
			{
				this->global_update.delta_weights[i] += update.examples_seen * update.delta_weights[i];
			}
		}

		for (std::int64_t i = 0;
			i < static_cast<std::int64_t>(this->global_update.network_structure.size()) - 1;
			i++)
		{
			this->global_update.delta_weights[i] *= (1.f / global_update.examples_seen);
		}
	}


} //namespace lindzei











/*
#define _CRTDBG_MAP_ALLOC

#include<stdlib.h>
#include <crtdbg.h>

#include <string.h>
#include <errno.h>

#include <pulse/cactuar.h>
#include <pulse/anima.h>
#include <orphan/federatedblock.h>

//May need to change 
//Could make a param but....
#define LOCAL_UPDATE_INIT_SIZE 64


federatedblock_t new_federated_block()
{
	blockheader_t *header = new_blockheader(0x01);

	network_update_t **local_updates = (network_update_t *)malloc(sizeof(network_update_t *) * LOCAL_UPDATE_INIT_SIZE);
	network_update_t *global_update = (network_update_t *)malloc(sizeof(network_update_t));

	//TOD: add more
	federatedblock_t block = {  .header = header,
								.local_update_count = 0,
								.local_update_size = LOCAL_UPDATE_INIT_SIZE,
								.local_updates = local_updates,
								.global_update = global_update
	};

	return block;
}

void mine_federated(federatedblock_t* const block, const unsigned char* const prev_hash)
{
	//TODO: remove cheat, used to spped up testing
	//mine(block->header, prev_hash);
	calculateGlobalUpdate(block);

}

int add_local_update(federatedblock_t *block, network_update_t *update)
{
	if (block->local_update_count == block->local_update_size)
	{
		block->local_update_size <<= 1;
		network_update_t **old_ptr = block->local_updates;
		block->local_updates = realloc(block->local_updates, block->local_update_size);
		if (block->local_updates == NULL)
		{
			//free_local_updates(old_ptr);
			block->local_updates = old_ptr;
			return errno;
		}
	}

	network_update_t *new_update = (network_update_t *)malloc(sizeof(network_update_t));
	if (new_update == NULL)
	{
		return errno;
	}
	memcpy(new_update, update, sizeof(network_update_t));

	block->local_updates[block->local_update_count] = new_update;
	block->local_update_count++;
}

//TODO: Just for now ??!!?!?
void get_global_update(federatedblock_t *block, network_update_t **update)
{
	*update = block->global_update;
}

network_update_t *new_global_update(int64_t num_layers, int64_t *network_dims)
{
	network_update_t *global_update = (network_update_t*)malloc(sizeof(network_update_t));
	if (global_update == NULL)
	{
		return NULL;
	}

	global_update->num_layers = num_layers;
	global_update->network_dims = (int64_t *)malloc(sizeof(int64_t) * (num_layers + 1)); ///<inc input neurons
	if (global_update->network_dims == NULL)
	{
		return NULL;
	}
	
	memcpy(global_update->network_dims, network_dims, sizeof(int64_t) * num_layers);

	global_update->delta_weights = (float**)malloc(sizeof(float*) * num_layers);
	if (global_update->delta_weights == NULL)
	{
		return NULL;
	}

	int64_t prev_layer_neurons = network_dims[0] + 1; // + 1 for bias
	int64_t next_layer_neurons;
	int64_t weight_size;
	for (int64_t i = 1; i <= num_layers; i++)
	{
		next_layer_neurons = network_dims[i];
		weight_size = prev_layer_neurons * next_layer_neurons;
		*(global_update->delta_weights + (i - 1)) = (float*)calloc(weight_size, sizeof(float));
		prev_layer_neurons = next_layer_neurons;
	}

	global_update->examples_seen = 0;
}



/*
network_update
	int64_t num_layers;
	int64_t *network_dims; ///< Must have @c num_layer many elements.
	float **delta_weights;
	int64_t examples_seen;

federated_block
	...
	int64_t local_update_count; ///< actual number of local updates assigned.
	int64_t local_update_size; ///< size of malloced local updates (first dimension).
	network_update_t **local_updates;
	network_update_t *global_update;
	...
*//*
int calculateGlobalUpdate(federatedblock_t * block)
{
	//TODO: check this is good!
	if (block->local_update_count == 0)
	{
		return -1;
	}
	//int64_t acc = 0;
	//TODO: assert all num layers and network_dims are equal!!
	int64_t num_layers = block->local_updates[0]->num_layers;
	int64_t *network_dims = block->local_updates[0]->network_dims;
	network_update_t *local_update;// var lu[]float32
	//num_layers?//variableCount : = len(block.LocalUpdates[0].Fst)
	network_update_t *global_update = new_global_update(num_layers, network_dims); //gu : = make([]float32, variableCount)

	int64_t prev_layer_neurons;
	int64_t next_layer_neurons;
	int64_t weight_size;

	float *scaled_weights;

	for (int64_t i = 0; i < block->local_update_count; i++)
		//_, v : = range block.LocalUpdates{
	{
		local_update = block->local_updates[i];
		global_update->examples_seen += local_update->examples_seen;

		prev_layer_neurons = network_dims[0] + 1; // + 1 for bias

		for (int64_t j = 1; j <= num_layers; j++)
		{
			next_layer_neurons = network_dims[j];

			weight_size = prev_layer_neurons * next_layer_neurons;

			scaled_weights = (float *)malloc(sizeof(float) * weight_size);

			scalar_vector_mult(local_update->examples_seen, local_update->delta_weights[j - 1],
				scaled_weights, weight_size);

			vector_vector_sum(global_update->delta_weights[j - 1], scaled_weights,
				global_update->delta_weights[j - 1], weight_size);

			free(scaled_weights);

			prev_layer_neurons = next_layer_neurons;
		}
	}

	prev_layer_neurons = network_dims[0] + 1; // + 1 for bias
	for (int64_t j = 1; j <= num_layers; j++)
	{
		next_layer_neurons = network_dims[j];

		weight_size = prev_layer_neurons * next_layer_neurons;

		scalar_vector_mult(1.f / global_update->examples_seen, global_update->delta_weights[j - 1],
			global_update->delta_weights[j - 1], weight_size);

		prev_layer_neurons = next_layer_neurons;
	}

	block->global_update = global_update;
}

*/