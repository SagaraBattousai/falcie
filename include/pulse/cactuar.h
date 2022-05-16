#ifndef __PULSE_CACTUAR_CACTAUR_H__
#define __PULSE_CACTUAR_CACTAUR_H__
#include <stdint.h>
#include <pulse/crypto.h>

#define CACTUAR_BASE_DIFFICULTY 0x1F0FFF1D
#define MULTIPLIER_MASK 0x00FFFFFF

//Same as bitcoin, used to scale the bits to bytes
#define CACTUAR_EXP_SCALAR 8
#define CACTUAR_EXP_SHIFTER 4
#define CACTUAR_EXP_SCALED_SHIFTER 32

/*-----------------------------------------------------------------------------
*  theta * 2^(8 * (alpha - 4))
*  223 * 8 is the absolute maximum it can be!
*  223 * 8 means calculations are for 256 blocks.
*  as 8 * (255 - 3) = 2016 (bitcoins block check)
*  32 //Vs bitcoins 24 (more small numbers?)
*  BlockCheckCount uint32 = 1004 // 4 * ( 255 - 4 ) Aka Cactuar exponential
*  ^^ Belongs to the Block / Chain not this class (for now) set block check
*  count to the length of the unrolled chain list which is of power 2
*----------------------------------------------------------------------------*/


typedef uint32_t cactuar_t;

/***********************************************************
* 
* @pre **bytes** must be zeroed array of size 32 
* (aka 32 bytes / 256 bits)
* 
* @param[in]  cactuar encoded value (used to store block difficulty) to 
* get the 256 bit encoding of.
* 
* @param[out] bytes   actual 256 bit value encoded by **cactuar**.
* This must be zeroed otherwise it will not work correctly, for example 
* using calloc or byte_t bytes[32] = {0} etc.
* This is in BigEndian format due to the hash leading zeros style.
* 
************************************************************/
void unravel_cactuar(const cactuar_t *const cactuar, unsigned char* bytes);

int targetcmp(const cactuar_t* const cactuar, const sha256hash_t* const hash);

float fast_log2(float val);

float frand(unsigned int *seed);


#endif