/*
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <cactuar/blockheader.h>
#include <cactuar/encoding.h>
#include <cactuar/crypto.h>
#include <cactuar/target.h>

#define NANO_TO_MILLI 1000000

blockheader_t *new_blockheader(uint32_t version)
{
	blockheader_t *header = (blockheader_t *)malloc(sizeof(blockheader_t));
	//nonce ~must/should be 0 and target will be set for now but can change when needed
	header->version = version;
	header->prev_hash = NULL;
	header->nonce = 0;
	header->target = CACTUAR_BASE_DIFFICULTY;//base_difficulty();

	return header;
}

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
	header->timestamp = generate_timestamp();

	//TODO: add error for invalid cactaur values
	unsigned char difficulty_array[32] = { 0 };
	unravel_cactuar(&header->target, difficulty_array); // blockHeader.Target.As256Bit()

	hash_t *currHash = hash(header);

	while (memcmp(currHash->hash, difficulty_array, 32) > 0) {
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

/*
int64_t generate_timestamp()
{
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0)
	{
		return 0;
	}
	int64_t timestamp = ((int64_t)ts.tv_sec) * 1000;
	timestamp += (int64_t)(ts.tv_nsec / NANO_TO_MILLI);
	return timestamp;
}
*/