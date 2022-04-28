
#include <etro/encoding.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "endianness_check.h"

/*
* These all seem to just be for convenience now I think about it. I mean tbh most of it is just byte
* swapping and convert_number and decode_number do almost the exact same thing!!
*/

void encode_number(const void* const number, unsigned char* encoded, size_t size)
{
	unsigned char* src = (unsigned char*)number;

	if (system_is_little_endian())
	{
		memcpy(encoded, src, size);
	}
	else
	{
		size_t final_byte_index = size - 1;
		for (int i = 0; i < size; i++)
		{ 
			*(encoded + i) = *(src + (final_byte_index - i));
		}
	}
}

unsigned char* convert_number(void* number, size_t size)
{
	unsigned char* encoded = (unsigned char*)number;

	if (system_is_big_endian())
	{
		size_t final_byte_index = size - 1;
		unsigned char tmp;
		for (int i = 0; i < size / 2; i++)
		{
			tmp = *(encoded + (final_byte_index - i));
			*(encoded + (final_byte_index - i)) = *(encoded + i);
			*(encoded + i) = tmp;
		}
	}
	return encoded;
}

void decode_number(unsigned char* encoded, size_t size) {

	if (system_is_big_endian())
	{
		size_t final_byte_index = size - 1;
		unsigned char tmp;
		for (int i = 0; i < size / 2; i++)
		{
			tmp = *(encoded + (final_byte_index - i));
			*(encoded + (final_byte_index - i)) = *(encoded + i);
			*(encoded + i) = tmp;
		}
	}
	return encoded;
}
