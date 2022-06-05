###################################################################
############ Orphan The Unseen Powerhouse Target ##################
###################################################################

set(ORPHAN_MODULE_ROOT "${LINDZEI_MODULE_ROOT}/orphan")

set(cpp_sources 
  "federated_block.cpp" 
  "federated_blockchain.cpp"
  "federated_network.cpp" 
  )

set(orphan_module 
  "${ORPHAN_MODULE_ROOT}/orphan.ixx"
  "${ORPHAN_MODULE_ROOT}/federated_block.ixx"
  "${ORPHAN_MODULE_ROOT}/federated_blockchain.ixx"
  "${ORPHAN_MODULE_ROOT}/federated_network.ixx"
  )

add_library(orphan STATIC ${orphan_module} ${cpp_sources})

if(MSVC)
  add_pseudo_module_impl_objects(orphan "orphan.cpp")
endif()

#Technically a user doesn't need pulse as this is built on top of pulse ?!??
target_link_libraries(orphan PRIVATE pulse)

