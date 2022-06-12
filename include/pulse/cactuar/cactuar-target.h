//module;

#ifndef __CACTUAR_TARGET__
#define __CACTUAR_TARGET__


#include <cstdint>
#include <memory>
//#include <cstring>

#include <stdexcept>


//#include <pulse/cactuar.h>
//#include <cactuar/crypto.h>

//export module cactuar:target;

#include <vector> //import <vector>;
#include <compare> //import <compare>;

//export 
namespace pulse
{
	class Target;

	constexpr std::strong_ordering operator<=>(const Target&, const Target&);


	/** Wrapper around std::uint32_t to act as a target for blockchain hash */
	class Target
	{
	public:
		constexpr Target();

		constexpr Target(std::uint32_t);

		static inline constexpr Target BaseTarget() { return Target{ Target::BaseDifficulty }; };

		constexpr const std::vector<std::byte> Expand() const;

		constexpr inline std::strong_ordering operator<=>(const std::vector<std::byte>& hash)
		{ return this->Expand() <=> hash; };

		static constexpr int TargetSize = 32; //< target array is truly 256 bits

	private:
		static constexpr std::uint32_t BaseDifficulty = 0x1F0FFF1D;

		static constexpr unsigned char ExpMin = 0x04;
		static constexpr unsigned char ExpMax = 0x21;

		static constexpr unsigned char TargetExponentShifter = 4; //because it operates on such

		/// Max target value = 0x21_FF_FF_FF, Min target value (obviously) = 0x00_00_00_00
		const std::uint32_t target;

	};
	constexpr Target::Target() : target(Target::BaseDifficulty) {}

	constexpr Target::Target(std::uint32_t target) : target(target) {}


	constexpr const std::vector<std::byte> Target::Expand() const
	{
		unsigned char exp = (this->target >> 24) & 255;

		if (exp > Target::ExpMax)
		{
			return std::vector<std::byte>(Target::TargetSize, std::byte{ 0xFF });
		}

		if (exp < Target::ExpMin)
		{
			//I think but since we have -4 .... hmmm Have a think about this
			return std::vector<std::byte>(Target::TargetSize, std::byte{ 0x00 });
		}

		auto arr_index = 31 - (exp - Target::TargetExponentShifter);

		std::vector<std::byte> target_array(Target::TargetSize);

		target_array[arr_index] = std::byte{ (target & 255) };
		target_array[arr_index - 1] = std::byte{ ((target >> 8) & 255) };
		target_array[arr_index - 2] = std::byte{ ((target >> 16) & 255) };

		return target_array;
	}

	constexpr std::strong_ordering operator<=>(const Target& lhs, const Target& rhs)
	{
		return lhs.Expand() <=> rhs.Expand();
	}

	
} //namespace pulse

#endif