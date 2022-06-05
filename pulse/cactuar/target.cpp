module;

#include <cstdint>
#include <cstddef>
#include <stdexcept>

module cactuar:target;

import <vector>;
import <algorithm>;

namespace pulse
{
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
		target_array[arr_index - 1] = std::byte{((target >> 8) & 255)};
		target_array[arr_index - 2] = std::byte{((target >> 16) & 255)};

		return target_array;
	}

	constexpr std::strong_ordering operator<=>(const Target& lhs, const Target& rhs)
	{
		return lhs.Expand() <=> rhs.Expand();
	}

} //namespace pulse


	// TODO: For target diff calc 
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