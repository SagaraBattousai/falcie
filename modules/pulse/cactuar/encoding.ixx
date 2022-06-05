/*----------------------------------------------------------
 * Note: We're assuming that the endianess of floats,
 *       which we're also assuming is IEEE-754,
 *       has the same endianess as integers on the platform.
-----------------------------------------------------------*/

module;

#include <cstddef>

export module cactuar:encoding;

import <span>;
import <algorithm>;
import <vector>;

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
	constexpr bool SystemIsLittleEndian()
	{
		return (*(const char*)&cactuar::endianTest) == cactuar::littleEndianValue;
	}

	constexpr bool SystemIsBigEndian()
	{
		return (*(const char*)&cactuar::endianTest) == cactuar::bigEndianValue;
	}

	/**
	 * Encodes data as little endian by copying the data either as
	 * is for little endian or in reverse for big endian.
	 */
	std::vector<std::byte> EncodeData(const std::span<const std::byte> data)
	{
		std::vector<std::byte> encoded(data.size());

		if (SystemIsLittleEndian())
		{
			std::copy(data.begin(), data.end(), encoded.begin());

		}
		else
		{
			std::copy(data.rbegin(), data.rend(), encoded.begin());
		}

		return encoded;
	}

	/**
	 * Encodes data as little endian (potentially
	 * overwriting data if the system is not little endian).
	 */
	void EncodeData(std::span<std::byte> data)
	{
		if (SystemIsBigEndian())
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
	void DecodeData(std::span<unsigned char>& data)
	{
		if (SystemIsBigEndian())
		{
			std::reverse(data.begin(), data.end());
		}
	}
} //namespace pulse
