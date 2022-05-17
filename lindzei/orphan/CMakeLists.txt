###################################################################
############ Orphan The Unseen Powerhouse Target ##################
###################################################################

set(target_name orphan)

set(c_sources "federatedblock.c")
set(cpp_sources "federated_blockchain.cpp")

set(headers "${LINDZEI_INCLUDE_ROOT}/${target_name}/federated_blockchain.h"
            "${LINDZEI_INCLUDE_ROOT}/${target_name}/federatedblock.h"
            )

add_library(${target_name} SHARED ${c_sources} ${cpp_sources})

target_sources(${target_name} PUBLIC
  FILE_SET public_headers 
  TYPE HEADERS 
  BASE_DIRS "${LINDZEI_INCLUDE_ROOT}"
  FILES
  ${headers}
  )

target_link_libraries(${target_name} PUBLIC pulse)





