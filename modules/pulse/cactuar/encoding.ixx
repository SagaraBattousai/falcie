/*----------------------------------------------------------
 * Note: We're assuming that the endianess of floats,
 *       which we're also assuming is IEEE-754,
 *       has the same endianess as integers on the platform.
-----------------------------------------------------------*/

module;

#include <span>
#include <algorithm>

export module cactuar:encoding;

namespace pulse
{
	namespace cactuar
	{
		constexpr int endianTest = 0xABCDEF37;
		constexpr int littleEndianValue = 0x37;
		constexpr int bigEndianValue = 0xAB;

	} //namespace pulse::cactuar
} //namespace pulse

export namespace pulse
{
	inline constexpr bool SystemIsLittleEndian()
	{
		return (*(const char*)&cactuar::endianTest) == cactuar::littleEndianValue;
	}

	inline constexpr bool SystemIsBigEndian()
	{
		return (*(const char*)&cactuar::endianTest) == cactuar::bigEndianValue;
	}

	// Possibly a little ott with the inlining but .. meh

	/**
	 * Encodes data as little endian (potentially
	 * overwriting data if the system is not little endian).
	 */
	inline void EncodeData(std::span<unsigned char> data)
	{
		if (SystemIsBigEndian)
		{
			std::reverse(data.begin(), data.end());
		}
	}

	/**
	 * Decodes a data from little endian (potentially overwriting
	 * the data if the system is not little endian).
	 *
	 * If the system is already little endian then encoded returned
	 * unchanged.
	 * Otherwise the data is converted to back into big endian.
	 */
	inline void DecodeData(std::span<unsigned char> data)
	{
		if (SystemIsBigEndian)
		{
			std::reverse(data.begin(), data.end());
		}
	}
} //namespace pulse
