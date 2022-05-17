###################################################################
################ Dahaka The Container Target ######################
###################################################################

set(target_name dahaka)

set(headers "${INTERNAL_INCLUDE_ROOT}/${target_name}/chain.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/blockchain.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/orderedlist.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/tree.h"
  )

add_library(${target_name} INTERFACE ${headers})
  
target_include_directories(${target_name} INTERFACE
  $<BUILD_INTERFACE:${INCLUDE_ROOT}>
  $<INSTALL_INTERFACE:include>
  )
