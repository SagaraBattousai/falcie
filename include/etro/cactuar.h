#ifndef __ETRO_CACTAUR_H__
#define __ETRO_CACTAUR_H__
#include <stdint.h>

#define BASE_DIFFICULTY 0x1F0FFF1D
#define MULTIPLIER_MASK 0x00FFFFFF

//Same as bitcoin, used to scale the bits to bytes
#define CACTUAR_EXP_SCALAR 8
#define CACTUAR_EXP_SHIFTER 4
#define CACTUAR_EXP_SCALED_SHIFTER 32
/****
*  223 * 8 is the absolute maximum it can be!
*  223 * 8 means calculations are for 256 blocks.
*  as 8 * (255 - 3) = 2016 (bitcoins block check)
*  32 //Vs bitcoins 24 (more small numbers?)
*  BlockCheckCount uint32 = 1004 // 4 * ( 255 - 4 ) Aka Cactuar exponential
*  ^^ Belongs to the Block / Chain not this class (for now) set block check
*  count to the length of the unrolled chain list which is of power 2
****/


typedef uint32_t cactuar;




#endif