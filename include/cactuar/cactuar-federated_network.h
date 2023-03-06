//module;
#ifndef __CACTUAR_FEDERATED_NETWORK__
#define __CACTUAR_FEDERATED_NETWORK__

#include <cstdint>

#include <vector>

#include <anima/anima-matrix.h>

namespace cactuar
{
	//template<typename T>
	//class 
	struct NetworkUpdate
	{
	//private:
		//std::vector<std::int64_t> network_structure;
		std::vector<anima::Matrix<float>> delta_weights{};
		std::int64_t examples_seen{ 0 };
	};
}

#endif