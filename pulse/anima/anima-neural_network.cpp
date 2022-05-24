module;

//#include <cstdint>
#include <cmath>
//#include <algorithm>

module anima:neural_network;

import <array>;
import <vector>;

namespace pulse
{
	float sigmoid(const float& pre_act)
	{
		return 1.f / (1 + std::exp(-pre_act));
	}

	float delta_sigmoid(const float& x)
	{
		float sig = sigmoid(x);
		return sig * (1 - sig);
	}


	
} //namespace pulse