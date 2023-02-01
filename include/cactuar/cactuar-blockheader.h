
#ifndef __CACTUAR_BLOCKHEADER_H__
#define __CACTUAR_BLOCKHEADER_H__


#include <cstdint>
#include <cstddef>

#include <vector>
#include <algorithm>
#include <utility>
#include <ostream>

#include <cactuar/cactuar-block.h>
#include <cactuar/cactuar-endianness.h>
#include <cactuar/cactuar-target.h>

namespace cactuar
{
	class Blockheader;

	std::ostream& operator<<(std::ostream& os, const Blockheader& header);

	class Blockheader
	{
	public:
//I hate giant constructors but somewhat needed for std::make_unique
		constexpr Blockheader(
			std::uint32_t version = 0x01,
			std::int64_t timestamp = -1,
			std::vector<std::byte> prev_hash = std::vector<std::byte>{},
			std::vector<std::byte> transaction_hash = std::vector<std::byte>{},
			cactuar::Target target = {},
			std::uint32_t nonce = 0)
			: version{ version }
			, timestamp{ timestamp }
			, prev_hash{ prev_hash }
			, transaction_hash{ transaction_hash }
			, target{ target }
			, nonce{ nonce }
		{ }

		constexpr Blockheader(Blockheader&& rhs) noexcept
			: version{ std::move(rhs.version) }
			, timestamp{ std::move(rhs.timestamp) }
			, prev_hash{ std::move(rhs.prev_hash) }
			, transaction_hash{ std::move(rhs.transaction_hash) }
			, target{ std::move(rhs.target) }
			, nonce{ std::move(rhs.nonce) }
		{ }


		//Can't be constexpr with gcc but totaly chill with msvc :'(
		static constexpr Blockheader Genisis(std::uint32_t version = 0x01)
		{
			return Blockheader{
				version,
				GenerateTimestamp(),
				std::vector<std::byte>(),
				std::vector<std::byte>(),
				0x00000000,
				0
			};
		}

		std::vector<std::byte> State() const;

		std::uint32_t version;
		std::int64_t timestamp;
		std::vector<std::byte> prev_hash;
		std::vector<std::byte> transaction_hash;
		cactuar::Target target;
		std::uint32_t nonce;

	};
}

#endif