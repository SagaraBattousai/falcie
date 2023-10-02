###################################################################
############### Cactuar The Blockchain Target #####################
###################################################################

set(CACTUAR_MODULE_ROOT "${MODULE_ROOT}/cactuar")

set(cpp_sources
  "account.cpp"
  "block.cpp"
  "blockchain.cpp"
  "blockheader.cpp"
  "contract_account.cpp"
  "federated_network.cpp"
  "rlp.cpp"
  "target.cpp"
  "transaction.cpp"
  "user_account.cpp"
  )

set(cactuar_module
  "${CACTUAR_MODULE_ROOT}/account.h"
  "${CACTUAR_MODULE_ROOT}/block.h"
  "${CACTUAR_MODULE_ROOT}/chain.h"
  "${CACTUAR_MODULE_ROOT}/blockchain.h"
  "${CACTUAR_MODULE_ROOT}/blockheader.h"
  "${CACTUAR_MODULE_ROOT}/contract_account.h"
  "${CACTUAR_MODULE_ROOT}/endianness.h"
  "${CACTUAR_MODULE_ROOT}/network_update.h"
  "${CACTUAR_MODULE_ROOT}/network_structure_update.h"
  "${CACTUAR_MODULE_ROOT}/network_flat_update.h"
  "${CACTUAR_MODULE_ROOT}/rlp.h"
  "${CACTUAR_MODULE_ROOT}/target.h"
  "${CACTUAR_MODULE_ROOT}/transaction.h"
  "${CACTUAR_MODULE_ROOT}/user_account.h"
  )

add_library(cactuar SHARED ${cactuar_module} ${cpp_sources})

target_link_libraries(cactuar PUBLIC crypto ndarray)

target_sources(cactuar PUBLIC
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${cactuar_module} 
  "${MODULE_ROOT}/_falcie_config.h"
  )

