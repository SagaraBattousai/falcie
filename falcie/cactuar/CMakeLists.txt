###################################################################
############ Orphan The Unseen Powerhouse Target ##################
###################################################################

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(ORPHAN_MODULE_ROOT "${LINDZEI_MODULE_ROOT}/orphan")

set(cpp_sources 
  "federated_block.cpp" 
  "federated_blockchain.cpp"
  "federated_blockheader.cpp" 
  "federated_contract_account.cpp" 
  "federated_network.cpp" 
  "federated_transaction.cpp" 
  "federated_user_account.cpp" 
  )

set(orphan_module 
  "${ORPHAN_MODULE_ROOT}/orphan.h" #ixx"
  "${ORPHAN_MODULE_ROOT}/federated_block.h" #ixx"
  "${ORPHAN_MODULE_ROOT}/federated_blockchain.h" #ixx"
  "${ORPHAN_MODULE_ROOT}/federated_blockheader.h" #ixx"
  "${ORPHAN_MODULE_ROOT}/federated_contract_account.h" #ixx"
  "${ORPHAN_MODULE_ROOT}/federated_network.h" #ixx"
  "${ORPHAN_MODULE_ROOT}/federated_transaction.h" #ixx"
  "${ORPHAN_MODULE_ROOT}/federated_user_account.h" #ixx"
  )

add_library(orphan OBJECT ${orphan_module} ${cpp_sources})

target_sources(orphan PUBLIC
  FILE_SET headers 
  TYPE HEADERS
  BASE_DIRS ${LINDZEI_MODULE_ROOT}
  FILES ${orphan_module}
  )

# if(MSVC)
#   intelisense_modules(orphan "${ORPHAN_MODULE_ROOT}")
# endif()

# Has to be public to allow passing of HashType etc
target_link_libraries(orphan PUBLIC pulse)
target_link_libraries(orphan PUBLIC cactuar)



