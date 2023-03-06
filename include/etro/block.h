#ifndef __ETRO_BLOCK_H__
#define __ETRO_BLOCK_H__

#ifdef __cplusplus
#include <cstdint>
#include <cactuar/cactuar-block.h>
#else
#include <stdint.h>
#endif

#include <etro/networkupdate.h>

//Typedefs for C vs CPP
#ifdef __cplusplus
typedef cactuar::Block block_t;
typedef cactuar::Block::Builder block_builder_t;
#else
typedef struct block block_t;
typedef struct block_builder block_builder_t;
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

	block_builder_t* new_block_builder(builder_params_t *params);

	block_t* build_block(block_builder_t *builder);

	block_t* build_genisis_block(block_builder_t *builder);

	int add_local_update(block_t *block, network_update_t *update);

	const char* block_as_string(block_t *block, size_t *str_size);

	//Won't return since we may add error handling to the C++ true code
	int get_global_update(block_t *block, network_update_t **update);

	void delete_builder(block_builder_t *builder);

	void delete_block(block_t *block);

#ifdef __cplusplus
}
#endif



#endif