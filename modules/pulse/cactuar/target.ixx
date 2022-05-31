module;


#include <cstdint>
#include <memory>
//#include <cstring>

//#include <pulse/cactuar.h>
//#include <cactuar/crypto.h>

export module cactuar:target;

import <vector>;
import <compare>;

namespace pulse
{
	namespace cactuar
	{
		constexpr std::uint32_t MULTIPLIER_MASK = 0x00FFFFFF;
	} //namespace pulse::cactuar
} //namespace pulse

export namespace pulse
{
	//TODO: try as std::byte as technically thats a far better description
	// Used to be called Cactuar but I reckon Target is a smarter name.
	//These may all be able to be constexpr much like stl
	class Target
	{
	public:

		explicit constexpr Target(std::uint32_t);

		static constexpr Target BaseTarget() { return Target(Target::BaseDifficulty); };

		friend inline constexpr std::strong_ordering operator<=>(
			const Target&, const std::vector<unsigned char>&);

		friend inline constexpr std::strong_ordering operator<=>(
			const Target&, const Target&);

	private:
		//may move outside ?!? Allowing the rest of cactuar to access
		static constexpr std::uint32_t BaseDifficulty = 0x1F0FFF1D;
		static constexpr int TargetSize = 32; //< target array is truly 256 bits

		/**
		 * Bitcoin uses theta * 2 ^ (8 * (alpha - 3)),
		 * where the 8 scales bits to bytes. We use
		 * theta * 2^(8 * (alpha - 4)) which (I think) gives
		 * us more small numbers.
		 *
		 * Bitcoin checks its difficulty every 2016 blocks
		 * i.e. 8 * (255 - 3). No idea what the rest of what I
		 * werote means.
		 * "every 223 * 8 means calculations are for 256 blocks.
		 * as" etc
		 */
		static constexpr std::uint32_t CACTUAR_EXP_SCALAR = 8;
		static constexpr std::uint32_t CACTUAR_EXP_SHIFTER = 4;
		static constexpr std::uint32_t CACTUAR_EXP_SCALED_SHIFTER = 32;

		//Could be an example of where std::byte is actually a smart move (since it's literally byte vals)
		//vector is probably more useable than unique[]
		//Set size of 32 bytes but vector so that allocation is on the heap.
		std::vector<unsigned char> target_array;
		const std::uint32_t target;

	};

	inline constexpr std::strong_ordering operator<=>(
		const Target& target, const std::vector<unsigned char>& hash)
	{
		return target.target_array <=> hash;
	}

	inline constexpr std::strong_ordering operator<=>(
		const Target& lhs, const Target& rhs)
	{
		return lhs.target_array <=> rhs.target_array;
	}

	

} //namespace pulse
