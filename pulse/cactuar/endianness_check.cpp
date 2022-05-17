#include <cactuar/endianness_check.h>

constexpr bool SystemIsLittleEndian()
{
	return (*(const char*)&endianTest) == littleEndianValue;
}

constexpr bool SystemIsBigEndian()
{
	return (*(const char*)&endianTest) == bigEndianValue;
}

bool system_is_little_endian()
{
	return SystemIsLittleEndian();
}

bool system_is_big_endian()
{
	return SystemIsBigEndian();
}