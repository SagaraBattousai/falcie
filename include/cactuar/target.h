//module;

#ifndef __CACTUAR_TARGET_H__
#define __CACTUAR_TARGET_H__


#include <cstdint>
#include <cstddef>
#include <memory>
//#include <cstring>

#include <stdexcept>


//#include <pulse/cactuar.h>
//#include <cactuar/crypto.h>

//export module cactuar:target;

#include <vector> //import <vector>;
#include <compare> //import <compare>;

#include <_falcie_config.h>

//export 
namespace cactuar
{
	//class Target;
#if __clang_minor__ < 15
	constexpr std::strong_ordering operator<=>(const std::vector<std::byte>& lhs,
			const std::vector<std::byte>& rhs)
	{
		auto lhs_size = lhs.size();
		auto rhs_size = rhs.size();

		if (lhs_size != rhs_size)
		{
			return lhs_size <=> rhs_size;
		}

		for (std::vector<std::byte>::size_type i = 0; i < lhs_size; ++i)
		{
			if ((unsigned char)lhs[i] != (unsigned char)rhs[i])
            {
                return (unsigned char)lhs[i] <=> (unsigned char)rhs[i];
            }
		}

        return std::strong_ordering::equal;
	}
	#endif

	/** Wrapper around std::uint32_t to act as a target for blockchain hash */
	class FALCIE_EXPORT Target
	{
	public:

		static constexpr std::uint32_t MinimumDifficulty = 0x21FFFFFF; //At least I think it's the min

		constexpr Target();

		constexpr Target(std::uint32_t);

		static inline constexpr Target BaseTarget() { return Target{ Target::BaseDifficulty }; };

		CONSTEXPR_FUNC inline const std::vector<std::byte> Expand() const;

		constexpr operator std::uint32_t() const { return target; }

		CONSTEXPR_FUNC inline std::strong_ordering operator<=>(const std::vector<std::byte>& hash)
		{ return (this->Expand()) <=> hash; }

		static constexpr int TargetSize = 32; //< target array is truly 256 bits

	private:
		static constexpr std::uint32_t BaseDifficulty = 0x1F0FFF1D;

		static constexpr unsigned char ExpMin = 0x04;
		static constexpr unsigned char ExpMax = 0x21;

		static constexpr unsigned char TargetExponentShifter = 4; //because it operates on such

		/// Max target value = 0x21_FF_FF_FF, Min target value (obviously) = 0x00_00_00_00
		std::uint32_t target;

	};
	constexpr Target::Target() : target(Target::BaseDifficulty) {}

	constexpr Target::Target(std::uint32_t target) : target(target) {}


	CONSTEXPR_FUNC const std::vector<std::byte> Target::Expand() const
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

		target_array[arr_index] = std::byte{ (unsigned char)(target & 255) };
		target_array[static_cast<std::int64_t>(arr_index) - 1] = std::byte{ (unsigned char)((target >> 8) & 255) };
		target_array[static_cast<std::int64_t>(arr_index) - 2] = std::byte{ (unsigned char)((target >> 16) & 255) };

		return target_array;
	}

	CONSTEXPR_FUNC inline std::strong_ordering operator<=>(const Target& lhs, const Target& rhs)
	{
		return (lhs.Expand()) <=> (rhs.Expand());
	}

} //namespace cactuar

#endif