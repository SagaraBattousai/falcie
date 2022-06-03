module;

#include <cstdint>
#include <stdexcept>

module cactuar:target;

import <vector>;

namespace pulse
{

	constexpr Target::expanded_target_type::size_type ExpandTargetIndex(const std::uint32_t& target)
	{
		unsigned char exp = (target >> 24) & 255;
		if (exp > 0x21 || exp < 0x04)
		{
			throw std::domain_error(
				"Target is either too large or too small. The leading byte of \"target\" \
					 must be less than or equal to 0x21 (33) and greater than or equal to 0x04 (4)"
			);
		}

		auto arr_index = 31 - (exp - Target::TargetExponentShifter);

		return arr_index;
	}

	const Target::expanded_target_type ExpandTarget(const std::uint32_t& target)
	{
		auto arr_index = ExpandTargetIndex(target);

		typename Target::expanded_target_type target_array(Target::TargetSize);

		target_array[arr_index] = (target & 255);
		target_array[arr_index - 1] = ((target >> 8) & 255);
		target_array[arr_index - 2] = ((target >> 16) & 255);

		return target_array;
	}
} //namespace pulse


	// Cant ramember why i need this VVVV
	/*
	* float fast_log2(float val);

	*
	#include <stdint.h>
	#include <stdlib.h>

	#include <cactuar/math.h>
	#include <cactuar/encoding.h>


	#define EXPONENT_OFFSET 23
	#define LOWER_BYTE_MASK 255
	#define EXPONENT_BIAS 127
	#define EXPONENT_CLEAR 0x807FFFFF ///< aka 1000 0000 0111 1111 1111 1111 1111 1111 in binary.
	#define EXPONENT_MASK 0x3F800000  ///< aka 0011 1111 1000 0000 0000 0000 0000 0000 in binary.

	//Bad names but since they're just polynomial constants that have been
	//calculated with remez they're somewhat arbitary
	#define A_COEFF -0.344845
	#define B_COEFF 2.024658
	#define C_COEFF 1.674873

	float fast_log2(float val) {
		uint32_t *bits = (uint32_t *) convert_data(&val, sizeof(float));

		//Below is the exponent, usually its exponent -1
		//but this time our mantissa calculation doesn't add 1
		//implicitly.
		uint32_t characteristic = (((*bits) >> EXPONENT_OFFSET) & LOWER_BYTE_MASK) - EXPONENT_BIAS;

		*bits &= EXPONENT_CLEAR;
		*bits += EXPONENT_MASK; // (is this the same as OR (||) since we zeroed it?)
		//val = (float*) bits;
		//mantissa = ((-1.0/3)*f+2)*f - 2.0/3 //The +1 version
		float mantissa = ((A_COEFF * val + B_COEFF) * val - C_COEFF);
		return characteristic + mantissa;
	}

	*/