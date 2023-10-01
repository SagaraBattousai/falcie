// module;
#ifndef __CACTUAR_NETWORK_STRUCTURE_UPDATE__
#define __CACTUAR_NETWORK_STRUCTURE_UPDATE__

#include <cactuar/cactuar-network_update.h>
#include <thoth/matrix.h>

#include <vector>

namespace cactuar {
// template<typename T>
// class
struct NetworkStructureUpdate : NetworkUpdate {
  // private:
  // std::vector<std::int64_t> network_structure;
  // new weight or change (delta)?
  std::vector<thoth::Matrix<float>> delta_weights{};
  int examples_seen{0};

  NetworkStructureUpdate() = default;
  NetworkStructureUpdate(std::vector<thoth::Matrix<float>> dw, int es)
      : delta_weights(dw), examples_seen(es){};
};
}  // namespace cactuar

#endif