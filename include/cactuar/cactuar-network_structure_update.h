//module;
#ifndef __CACTUAR_NETWORK_STRUCTURE_UPDATE__
#define __CACTUAR_NETWORK_STRUCTURE_UPDATE__

#include <cstdint>

#include <vector>

#include <cactuar/cactuar-network_update.h>
#include <anima/anima-matrix.h>

namespace cactuar
{
	//template<typename T>
	//class 
	struct NetworkStructureUpdate : NetworkUpdate
	{
	//private:
		//std::vector<std::int64_t> network_structure;
		std::vector<anima::Matrix<float>> delta_weights{};
		std::int64_t examples_seen{ 0 };
	};
}

#endif