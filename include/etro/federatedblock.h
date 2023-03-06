#ifndef __ETRO_FEDERATED_BLOCK_H__
#define __ETRO_FEDERATED_BLOCK_H__

#ifdef __cplusplus
#include <cstdint>
#include <cactuar/cactuar-block.h>
#else
#include <stdint.h>
#endif

#include <etro/networkupdate.h>

//Typedefs for C vs CPP
#ifdef __cplusplus
typedef cactuar::Block federated_block_t;
typedef cactuar::Block::Builder federated_block_builder_t;
#else
typedef struct federated_block federated_block_t;
typedef struct federated_block_builder federated_block_builder_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//Find a smarter way to do this other than copy and paste (i.e. extern c etc)
	typedef enum hash_algorithm { SHA256 = 32, RIPEMD160 = 20 } hash_algorithm_t;

	//typedef const blockhash_type(*block_hash_func)(hash_algorithm hash_algo, const federated_block_t* const);

	/*
	typedef struct blockhash_type
	{
		//VV Closest C equivelent to C++'s -> std::vector<std::byte> ( use reinterpret_cast<unsigned char *>)
		unsigned char* hash;
		int64_t hash_size;
	} blockhash_type_t;
	*/

	typedef struct builder_params
	{
		uint32_t version;

		uint32_t target;

		//block_hash_func hash_func; //For now force to be pulse::pulse_hash

		hash_algorithm_t hash_algo;

	} builder_params_t;

	//just calls the C++ version
	//inline const blockhash_type pulse_hash(hash_algorithm hash_algo, const federated_block_t* const);
	//^^ For now force this to be default / only

	federated_block_builder_t* new_federeated_block_builder(builder_params_t *params);

	federated_block_t* build_federated_block(federated_block_builder_t *builder);

	federated_block_t* build_genisis_block(federated_block_builder_t *builder);

	int add_local_update(federated_block_t *block, network_update_t *update);

	const char* block_as_string(federated_block_t *block, size_t *str_size);

	//Won't return since we may add error handling to the C++ true code
	int get_global_update(federated_block_t *block, network_update_t **update);

	void delete_builder(federated_block_builder_t *builder);

	void delete_block(federated_block_t *block);

#ifdef __cplusplus
}
#endif



#endif