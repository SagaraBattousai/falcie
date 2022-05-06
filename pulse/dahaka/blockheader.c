
#include <time.h>
#include <string.h>

#include <etro/blockheader.h>
#include <etro/encoding.h>
#include <etro/crypto.h>

#define NANO_TO_MILLI 1000000

void mine(blockheader_t* const header, const sha256hash_t* const prev_hash)
{
	header->prev_hash = *prev_hash;
	header->timestamp = generate_timestamp();

	//TODO: add error for invalid cactaur values
	byte_t difficulty_array[32] = { 0 };
	unravel_cactuar(&header->target, difficulty_array); // blockHeader.Target.As256Bit()

	sha256hash_t currHash = hash(header);

	while (memcmp(currHash.hash, difficulty_array, 32) > 0) {
		header->nonce += 1;
		currHash = hash(header);
	}


}

sha256hash_t hash(const blockheader_t* const header)
{
	sha256hash_t hash;// = { 0 };

	byte_t encoded[sizeof(blockheader_t)] = { 0 };
	encode_data(header, encoded, sizeof(blockheader_t));

	//TODO: Clean this up!
	hasher_t *hasher = get_default_sha256_hasher();
	
	/*
	update_hash(hasher, encoded, sizeof(blockheader_t));
	output_hash(hasher, hash.hash, NULL);
	* ^^ is the same as VV
	*/
	hash_data(hasher, encoded, sizeof(blockheader_t), hash.hash, NULL);

	return hash;
}

uint64_t generate_timestamp()
{
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0)
	{
		return 0;
	}
	uint64_t timestamp = ((uint64_t)ts.tv_sec) * 1000;
	timestamp += (uint64_t)(ts.tv_nsec / NANO_TO_MILLI);
	return timestamp;
}
