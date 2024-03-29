/*----------------------------------------------------------
 * Note: We're assuming that the endianess of floats,
 *       which we're also assuming is IEEE-754,
 *       has the same endianess as integers on the platform.
-----------------------------------------------------------*/

//module;
#ifndef __CACTUAR_ENDIANNESS_H__
#define __CACTUAR_ENDIANNESS_H__

#include <cstddef>

#include <span>
#include <algorithm>
#include <vector>

#include <_falcie_config.h>

namespace cactuar
{
	namespace
	{
		constexpr int endianTest = 0xABCDEF37;
		constexpr int littleEndianValue = 0x37;
		constexpr int bigEndianValue = 0xAB;

	}

	CONSTEXPR_FUNC inline bool SystemIsLittleEndian()
	{
		return (*(const char*)&endianTest) == littleEndianValue;
	}

	CONSTEXPR_FUNC inline bool SystemIsBigEndian()
	{
		return (*(const char*)&endianTest) == bigEndianValue;
	}

	//Bit big for inline but really just if and copy.
	/**
	 * Encodes data as little endian by copying the data either as
	 * is for little endian or in reverse for big endian.
	 */
	inline std::vector<std::byte> EncodeData(const std::span<const std::byte> data)
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
	inline void EncodeData(std::span<std::byte> data)
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
	inline void DecodeData(std::span<unsigned char>& data)
	{
		if (SystemIsBigEndian())
		{
			std::reverse(data.begin(), data.end());
		}
	}

	//std::vector<std::byte>::iterator OutIT should have underlying std::byte
	template< typename T, typename OutputIt >
	constexpr OutputIt CopyDataAsBytes(const T& data, OutputIt result)
	{
		return std::copy_n(reinterpret_cast<const std::byte*>(&data), sizeof(T), result);
	}


} //namespace cactuar

#endif