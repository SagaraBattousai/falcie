#include <etro/cactuar.h>

//In BigEndian format due to the hash leading zeros style
//Pre: bytes must be zeroed
void unravel_cactuar(const cactuar_t *const cactuar, byte_t* bytes)
{
	byte_t exp = (*cactuar >> 24) & 255;
	uint32_t zero_byte_count = exp - CACTUAR_EXP_SHIFTER;
	uint32_t arr_index = 31 - zero_byte_count;

	bytes[arr_index] = (*cactuar & 255);
	bytes[arr_index - 1] = ((*cactuar >> 8) & 255);
	bytes[arr_index - 2] = ((*cactuar >> 16) & 255);

}