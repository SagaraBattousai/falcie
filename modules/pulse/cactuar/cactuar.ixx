
export module cactuar;

/*************************************************************
************** Cactuar's Public API Header *******************
**************************************************************/

/*

#include <stdint.h>

/// Value is the size of the type in bytes
enum hash_type_t {SHA256 = 32,  RIPEMD160 = 20};

typedef uint32_t cactuar_t;

//typedef struct hash hash_t;
//TODO: Remove, only for debugging!
//That said, may want to expose as its somewhat useful ....
typedef struct hash
{
    int size;
    unsigned char *hash;
} hash_t;

//typedef struct blockheader blockheader_t;
//TODO: Remove, only for debugging!
//That said, may want to expose as its somewhat useful ....
typedef struct blockheader
{
    uint32_t version;
    int64_t timestamp;
    hash_t *prev_hash;
    hash_t *transaction_hash;
    uint32_t target;
    uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
} blockheader_t;

blockheader_t *new_blockheader(uint32_t version);

hash_t *get_prev_hash(blockheader_t *header);

cactuar_t *get_target(blockheader_t *header);

int targetcmp(const cactuar_t* const cactuar, const unsigned char* const hash);

void mine(blockheader_t * const header, const unsigned char* const prev_hash);

hash_t *hash(const blockheader_t* const header);

int64_t generate_timestamp();

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

//int64_t generate_timestamp();


#endif
#ifndef __PULSE_CACTUAR_CRYPTO_H__
#define __PULSE_CACTUAR_CRYPTO_H__

//#include <stdint.h>

#include <pulse/cactuar.h>

*/
/*
typedef struct sha256_hash {
	unsigned char hash[SHA256];
}sha256hash_t;

typedef struct ripemd160_hash {
	unsigned char size = RIPEMD160;
	unsigned char hash[RIPEMD160];
}ripemd160hash_t;
*/

/*
typedef struct hash
{
	int size;
	unsigned char *hash;
} hash_t;
*/

/*
typedef struct hasher hasher_t;


hasher_t* get_default_sha256_hasher();
*/
/** size_t required for compatability with OpenSSL */
//inline void update_hash(hasher_t* hasher, const void* data, size_t count);

/** unsigned int required for compatability with OpenSSL */
//inline void output_hash(hasher_t* hasher, unsigned char* dst, unsigned int* size);


//Potentiall make inline, combo of the two above sans the func calls.
/** size_t, unsigned int required for compatability with OpenSSL */

/*
void hash_data(hasher_t* hasher, const void* src, size_t src_count,
	unsigned char* dst, unsigned int* dst_size);

void free_hasher(hasher_t* hasher);


#endif
#ifndef __PULSE_CACTUAR_ENCODING_H__
#define __PULSE_CACTUAR_ENCODING_H__

#include <stddef.h>
*/
/*----------------------------------------------------------
* Note: We're assuming that the endianess of floats, 
*       which we're also assuming is IEEE-754, 
*       has the same endianess as integers on the platform.
------------------------------------------------------------*/



/*************************************************************
* Encodes a data as little endian.
* 
* If the system is already little endian then **data** is just 
* copied into **encoded** with a pointer cast.
* 
* encoded's value is decoupled from **data** and therefore will not 
* have any unintended side effects 
* 
* @param[in]  data  is a ptr to a data to be encoded
* @param[out] encoded must have at least size bytes and will point to data in
* little endian order.
* @param[in]  size    is the size of the data of bytes "data" has
***************************************************************/
//void encode_data(const void *const data, unsigned char *encoded, size_t size);

/******************************************************************************
* Converts a data as little endian (overwriting data)
* 
* If the system is already little endian then **data** is just cast to 
* and unsigned char. This is faster than **encode_data** but in the case
* the system is not little endian, data will have the wrong format and will
* not contain the same value passed in.
* 
* The returned pointer's lifetime is dependant upon data as it points to the same location.
* 
*
* The returned pointer points to **data** and therefore there may be unintended side effects
* when modifying its value. However this can also be of use, we use this to mdoify a 
* floating point data's binary representation in math.c
* 
* The returned pointer can also be passed into **decode_data** but again this will modify
* **data**
* 
* @param[in] data is a ptr to a data to be converted
* @param[in] size   is the size of the data of bytes **data** has.
* @return The returned pointer will live only as long as **data**
*         does (since it just points to it).
******************************************/
//unsigned char* convert_data(void* data, size_t size);


/*************************************************************
* Decodes a data from little endian.
*
* If the system is already little endian then encoded returned
* unchanged. 
* Otherwise the data is converted to back into big endian. Note that is
* encoded was generated by **convert_data** then the **data** argument
* passed in will also be modified by this function as long as encoded
* still points to data (as would be the case in **convert_data**.
* 
* decode_data does almost exactly the same thing as convert data,
* its primarily for increased code readability, avoids one useless cast and
* is somewhat more type tidy.
*
* @param[in,out] encoded must have at least size bytes and its bytes will be
* translated back into the systems endianness.
* @param[in] size is the size of the data of bytes "data" has
***************************************************************/
//void decode_data(unsigned char* encoded, size_t size);

/*
#ifdef __cplusplus
constexpr int endianTest = 0xABCDEF37;
constexpr int littleEndianValue = 0x37;
constexpr int bigEndianValue = 0xAB;
constexpr bool SystemIsLittleEndian();
constexpr bool SystemIsBigEndian();
extern "C" {
#else
#include <stdbool.h>
#endif

extern inline bool system_is_little_endian();
extern inline bool system_is_big_endian();

#ifdef __cplusplus
}
#endif


#endif
#ifndef __PULSE_CACTUAR_MATH_H__
#define __PULSE_CACTUAR_MATH_H__

float fast_log2(float val);

#endif
#ifndef __PULSE_CACTUAR_TARGET_H__
#define __PULSE_CACTUAR_TARGET_H__

#include <stdint.h>

#include <pulse/cactuar.h>
#include <cactuar/crypto.h>

#define CACTUAR_BASE_DIFFICULTY 0x1F0FFF1D
#define MULTIPLIER_MASK 0x00FFFFFF

//Same as bitcoin, used to scale the bits to bytes
#define CACTUAR_EXP_SCALAR 8
#define CACTUAR_EXP_SHIFTER 4
#define CACTUAR_EXP_SCALED_SHIFTER 32

*/
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
/*
void unravel_cactuar(const cactuar_t *const cactuar, unsigned char* bytes);

*/
