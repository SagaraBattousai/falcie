
#include <vector>

#include <anima/anima-matrix.h>
#include <orphan/federated_network.h>

//export
namespace lindzei
{
	/*
	NetworkUpdate GlobalNetworkUpdate(std::int64_t weight_count)
	{
		std::vector<pulse::Matrix>

		//Copies network_structure I hope
		NetworkUpdate globalUpdate{ delta_weights = pulse::NetworkWeights{},
									 .examples_seen = 0
		};

		globalUpdate.delta_weights.reserve(network_structure.size() - 1);

		std::vector<std::int64_t>::const_iterator it = network_structure.begin();

		std::int64_t previous_layer_nodes = *it + 1; //+1 for bias
		std::int64_t next_layer_nodes;
		//std::int64_t weight_size;
		++it;
		for (; it != network_structure.end(); ++it)
		{
			next_layer_nodes = *it;
			//weight_size = previous_layer_nodes * next_layer_nodes;
			globalUpdate.delta_weights.emplace_back(
				pulse::Dimensions{ previous_layer_nodes, next_layer_nodes }
			);
			previous_layer_nodes = next_layer_nodes;
		}

		return globalUpdate;
	}
	*/
} // namespace lindzei