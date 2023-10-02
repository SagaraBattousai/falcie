
#ifndef __CACTUAR_BLOCKHEADER_H__
#define __CACTUAR_BLOCKHEADER_H__


#include <cstdint>
#include <cstddef>
#include <chrono> //import <chrono>;

#include <vector>
#include <algorithm>
#include <utility>
#include <ostream>

#include <cactuar/endianness.h>
#include <cactuar/target.h>

namespace cactuar
{
	class Blockheader;

	std::ostream& operator<<(std::ostream& os, const Blockheader& header);

	class Blockheader
	{
	public:

		using time_rep = std::int64_t; //Maybe make private
		using time_period = std::micro; //Maybe make private
		using time_duration = std::chrono::duration<time_rep, time_period>;
		using time_clock = std::chrono::system_clock; //std::chrono::tai_clock;
		using time_point = std::chrono::time_point<time_clock, time_duration>;

		static time_rep GenerateTimestamp()
		{
			return std::chrono::duration_cast<time_duration>
				(time_clock::now().time_since_epoch()).count();
		};

		//TODO: May need optimising
		static time_point AsTimePoint(const std::int64_t& timestamp)
		{
			return time_point(time_duration(timestamp));
		}


//I hate giant constructors but somewhat needed for std::make_unique
		//constexpr 
			Blockheader(
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

		//constexpr 
			Blockheader(Blockheader&& rhs) noexcept
			: version{ std::move(rhs.version) }
			, timestamp{ std::move(rhs.timestamp) }
			, prev_hash{ std::move(rhs.prev_hash) }
			, transaction_hash{ std::move(rhs.transaction_hash) }
			, target{ std::move(rhs.target) }
			, nonce{ std::move(rhs.nonce) }
		{ }


		//Can't be constexpr with gcc but totaly chill with msvc :'(
		//static constexpr 
			static Blockheader Genisis(std::uint32_t version = 0x01)
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