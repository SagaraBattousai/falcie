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
  "${CACTUAR_MODULE_ROOT}/cactuar-account.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-block.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-chain.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-blockchain.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-blockheader.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-contract_account.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-endianness.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-network_update.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-network_structure_update.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-network_flat_update.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-rlp.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-target.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-transaction.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-user_account.h"
  )

add_library(cactuar SHARED ${cactuar_module} ${cpp_sources})

target_link_libraries(cactuar PUBLIC orphan thoth)
# target_include_directories(cactuar PUBLIC 
#   "${PROJECT_SOURCE_DIR}/falcie/orphan/include"
#   ${MODULE_ROOT})

target_sources(cactuar PUBLIC
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${cactuar_module} 
  "${MODULE_ROOT}/_falcie_config.h"
  )

