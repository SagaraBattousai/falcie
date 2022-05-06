#ifndef __ETRO_ENDIANNESS_CHECK_H__
#define __ETRO_ENDIANNESS_CHECK_H__

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