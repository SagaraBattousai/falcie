module;


#include <cstdint>
#include <memory>
//#include <cstring>

#include <stdexcept>


//#include <pulse/cactuar.h>
//#include <cactuar/crypto.h>

export module cactuar:target;

import <vector>;
import <compare>;

export namespace pulse
{
	//class Target;

	//TODO: try as std::byte as technically thats a far better description
	// Used to be called Cactuar but I reckon Target is a smarter name.

	/** Wrapper around std::uint32_t to act as a target for blockchain hash */
	class Target
	{
	public:

		using expanded_target_type = std::vector<unsigned char>;

		//Make potentiall implicit.... so we can treat uint32_t's as targets.
		constexpr Target(std::uint32_t);

		static inline constexpr Target BaseTarget() { return Target(Target::BaseDifficulty); };

		// Both operators are messy af but are constexp so that's nice right? 
		// Or expanded target type????
		constexpr std::strong_ordering operator<=>(const std::vector<unsigned char>&) const;
		friend constexpr std::strong_ordering operator<=>(const Target&, const Target&);

		static constexpr int TargetSize = 32; //< target array is truly 256 bits
		static constexpr unsigned char TargetExponentShifter = 4; //because it operates on such

	private:
		static constexpr std::uint32_t BaseDifficulty = 0x1F0FFF1D;

		/// Max target value = 0x21_FF_FF_FF, Min target value (obviously) = 0x00_00_00_00
		const std::uint32_t target;

	};

	constexpr Target::expanded_target_type::size_type ExpandTargetIndex(const std::uint32_t& target);

	//could put in cactuar ns
	const Target::expanded_target_type ExpandTarget(const std::uint32_t& target);

	constexpr Target::Target(std::uint32_t target) : target(target) {}

	// Both operators are messy af but are constexp so that's nice right? 
	constexpr std::strong_ordering Target::operator<=>(const std::vector<unsigned char>& hash) const
	{
		//constexpr 
		expanded_target_type target_array(TargetSize);

		auto arr_index = ExpandTargetIndex(this->target);

		target_array[arr_index] = (target & 255);
		target_array[arr_index - 1] = ((target >> 8) & 255);
		target_array[arr_index - 2] = ((target >> 16) & 255);

		return target_array <=> hash;
	}

	constexpr std::strong_ordering operator<=>(const Target& lhs, const Target& rhs)
	{
		Target::expanded_target_type lhs_target_array(Target::TargetSize);
		Target::expanded_target_type rhs_target_array(Target::TargetSize);

		auto lhs_arr_index = ExpandTargetIndex(lhs.target);
		auto rhs_arr_index = ExpandTargetIndex(rhs.target);

		lhs_target_array[lhs_arr_index] = (lhs.target & 255);
		lhs_target_array[lhs_arr_index - 1] = ((lhs.target >> 8) & 255);
		lhs_target_array[lhs_arr_index - 2] = ((lhs.target >> 16) & 255);
		
		rhs_target_array[rhs_arr_index] = (rhs.target & 255);
		rhs_target_array[rhs_arr_index - 1] = ((rhs.target >> 8) & 255);
		rhs_target_array[rhs_arr_index - 2] = ((rhs.target >> 16) & 255);

		return lhs_target_array <=> rhs_target_array;
	}



} //namespace pulse
