//#include <utility>
#include <cactuar/cactuar-crypto.h>

namespace pulse
{
	std::ostream& operator<<(std::ostream& os, const HashAlgorithm& hashAlgo)
	{
		switch (hashAlgo)
		{
		case HashAlgorithm::SHA256:
			os << "SHA256";
			break;
		case HashAlgorithm::RIPEMD160:
			os << "RIPEMD160";
			break;
		default:
			os << "UNKNOWN HASH ALGORITHM";
		}

		return os;
	}

	std::ostream& operator<<(std::ostream& os, const ElipticalCurve& curve)
	{
		switch (curve)
		{
		case ElipticalCurve::SECP256K1:
			os << "SECP256K1";
			break;
		default:
			os << "UNKNOWN ELIPTICAL CURVE";
		}

		return os;
	}
}

