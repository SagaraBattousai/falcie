module;

#include <cstdint>

export module orphan:federated_network;

import <vector>;

import pulse;


class NetworkUpdate
{
	std::vector<std::int64_t> network_structure;

	pulse::NetworkWeights delta_weights;
	int64_t examples_seen;

};
