#ifndef __ETRO_ENCODING_H__
#define __ETRO_ENCODING_H__

#include <stddef.h>

//encodes a number as little endian
//param size is the size of the number of bytes "number" has
//param encoded muct have at least size bytes
void encode_number(void *number, unsigned char *encoded, size_t size);

//converts a number as little endian (overwriting number)
//param size is the size of the number of bytes "number" has
//return value will live only as long as number does (since it just points to it).
unsigned char* convert_number(void* number, size_t size);


#endif