###################################################################
############ Orphan The Unseen Powerhouse Target ##################
###################################################################

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(CACTUAR_MODULE_ROOT "${MODULE_ROOT}/cactuar")

set(cpp_sources
  "account.cpp"
  "block.cpp"
  # "blockchain.cpp"
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
  "${CACTUAR_MODULE_ROOT}/cactuar-blockchain.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-blockheader.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-contract_account.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-endianness.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-federated_network.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-rlp.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-target.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-transaction.h"
  "${CACTUAR_MODULE_ROOT}/cactuar-user_account.h"
  )

add_library(cactuar SHARED ${cactuar_module} ${cpp_sources})

target_link_libraries(cactuar PUBLIC orphan anima)

target_sources(cactuar PUBLIC
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${cactuar_module}
  )

# if(MSVC)
#   intelisense_modules(cactuar "${CACTUAR_MODULE_ROOT}")
# endif()

# Has to be public to allow passing of HashType etc
# target_link_libraries(cactuar PUBLIC pulse)
# target_link_libraries(cactuar PUBLIC cactuar)



