module;

#include <cstdint>

export module cactuar:blockheader;

import <vector>;
import <chrono>;
import <ratio>;
import <functional>;

export namespace pulse
{
	class Blockheader;

	using BlockHasher = std::function<Blockheader::hash_t(const Blockheader&)>;

	class Blockheader
	{
	public:
		constexpr Blockheader(BlockHasher, uint32_t version = 0x01);

		using time_rep = uint64_t;
		using time_period = std::micro;
		using time_duration = std::chrono::duration<time_rep, time_period>;

		//ToDO: potentially rename
		using hash_t = std::vector<unsigned char>;

	private:

		static time_rep GenerateTimestamp();

		uint32_t version;
		int64_t timestamp;
		hash_t prev_hash;
		hash_t transaction_hash;
		uint32_t target;
		uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
	};

	Blockheader::Blockheader(uint32_t version)
		: version(version)
	{
	}

	Blockheader::time_rep Blockheader::GenerateTimestamp()
	{
		return std::chrono::duration_cast<Blockheader::time_duration>
			(std::chrono::tai_clock::now().time_since_epoch()).count();

	}
}

	/*
	hash_t *get_prev_hash(blockheader_t *header)
	{
		return header->prev_hash;
	}

	cactuar_t *get_target(blockheader_t *header)
	{
		return header->target;
	}




	void mine(blockheader_t* const header, const unsigned char* const prev_hash)
	{
		//Shouldn't ever happen
		if (header->prev_hash != NULL)
		{
			free(header->prev_hash);
		}
		header->prev_hash = (hash_t *)malloc(sizeof(hash_t));
		header->prev_hash->hash = prev_hash;
		//TODO: Remove as a cheat
		header->prev_hash->size = 0x20;
		header->timestamp = GenerateTimestamp();

		//TODO: add error for invalid cactaur values
		unsigned char difficulty_array[32] = { 0 };
		unravel_cactuar(&header->target, difficulty_array); // blockHeader.Target.As256Bit()

		hash_t *currHash = hash(header);

		while (memcmp(currHash->hash, difficulty_array, 32) > 0)
		{
			header->nonce += 1;
			free_hash(currHash);
			currHash = hash(header);
		}


	}
	*/
		//TODO: decide if hash hash_type is optional
		//hash_t *hash(const blockheader_t* const header)
		//{
		//	hash_t *hash = (hash_t *)malloc(sizeof(hash_t));
		//	hash->size = SHA256;
		//	hash->hash = (unsigned char *)malloc(sizeof(unsigned char) * hash->size);

		//	unsigned char encoded[sizeof(blockheader_t)] = { 0 };
		//	encode_data(header, encoded, sizeof(blockheader_t));

			//TODO: Clean this up!
		//	hasher_t *hasher = get_default_sha256_hasher();

			/*
			update_hash(hasher, encoded, sizeof(blockheader_t));
			output_hash(hasher, hash.hash, NULL);
			* ^^ is the same as VV
			*/

			//	hash_data(hasher, encoded, sizeof(blockheader_t), hash->hash, NULL);

			//	return hash;
			//}

} //namespace pulse

//#include <stdint.h>


/*
//typedef struct blockheader blockheader_t;
//TODO: Remove, only for debugging!
//That said, may want to expose as its somewhat useful ....


blockheader_t *new_blockheader(uint32_t version);

hash_t *get_prev_hash(blockheader_t *header);

cactuar_t *get_target(blockheader_t *header);

int targetcmp(const cactuar_t* const cactuar, const unsigned char* const hash);

void mine(blockheader_t * const header, const unsigned char* const prev_hash);

hash_t *hash(const blockheader_t* const header);

int64_t GenerateTimestamp();

//extern inline cactuar_t base_difficulty();

void free_hash(hash_t *hash);



#endif
#ifndef __ETRO_BLOCKHEADER_H__
#define __ETRO_BLOCKHEADER_H__

#include <stdint.h>

#include <pulse/cactuar.h>
#include <cactuar/crypto.h>

*/
/*******************************************************************
* The header for  block on the chain which is hashed to provide PoW
********************************************************************/
/*
typedef struct blockheader
{
	uint32_t version;
	int64_t timestamp;
	hash_t *prev_hash;
	hash_t *transaction_hash;
	uint32_t target;
	uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
} blockheader_t;
*/

/*
hash_t* hash(const blockheader_t* const header);

//int64_t GenerateTimestamp();


#endif


/*******************************************************************
* The header for  block on the chain which is hashed to provide PoW
********************************************************************/
/*
typedef struct blockheader
{
	uint32_t version;
	int64_t timestamp;
	hash_t *prev_hash;
	hash_t *transaction_hash;
	uint32_t target;
	uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
} blockheader_t;
*/


//hash_t* hash(const blockheader_t* const header);

//int64_t GenerateTimestamp();

