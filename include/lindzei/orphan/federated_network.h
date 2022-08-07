//module;
#ifndef __ORPHAN_FEDERATED_NETWORK__
#define __ORPHAN_FEDERATED_NETWORK__

#include <cstdint>

//export module orphan:federated_network;

#include <vector> //import <vector>;

//import pulse;
#include <pulse/pulse.h>

//export 
namespace lindzei
{
	//class 
	struct NetworkUpdate
	{
		pulse::NetworkWeights delta_weights;
		std::int64_t examples_seen{ 0 };
	};

}

#endif