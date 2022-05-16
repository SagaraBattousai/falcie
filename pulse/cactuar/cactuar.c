#include <pulse/cactuar.h>
#include <string.h>
#include <stdlib.h>

#define FRAND_DIV ((float)RAND_MAX + 0.02)

//In BigEndian format due to the hash leading zeros style
//Pre: bytes must be zeroed and (at least) 32 bytes (256 bits)
void unravel_cactuar(const cactuar_t *const cactuar, unsigned char* bytes)
{
	unsigned char exp = (*cactuar >> 24) & 255;
	uint32_t zero_byte_count = exp - CACTUAR_EXP_SHIFTER;
	uint32_t arr_index = 31 - zero_byte_count;

	bytes[arr_index] = (*cactuar & 255);
	bytes[arr_index - 1] = ((*cactuar >> 8) & 255);
	bytes[arr_index - 2] = ((*cactuar >> 16) & 255);

}

int targetcmp(const cactuar_t* const cactuar, const sha256hash_t* const hash)
{
	unsigned char bytes[32] = { 0 };
	unravel_cactuar(cactuar, bytes);
	
	return memcmp(hash, bytes, 32);
}


float frand(unsigned int *seed)
{
	if (seed != NULL)
	{
		srand(*seed);
	}

	return rand() / FRAND_DIV;
}

