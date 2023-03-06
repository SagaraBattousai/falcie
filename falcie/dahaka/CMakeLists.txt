###################################################################
################ Dahaka The Container Target ######################
###################################################################

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(DAHAKA_MODULE_ROOT "${MODULE_ROOT}/dahaka")

set(dahaka_module 
  "${DAHAKA_MODULE_ROOT}/dahaka.h" # ixx"
  "${DAHAKA_MODULE_ROOT}/dahaka-chain.h" # ixx" 
  "${DAHAKA_MODULE_ROOT}/dahaka-blockchain.h" # ixx" 
  "${DAHAKA_MODULE_ROOT}/dahaka-orderedlist.h" # ixx" 
  "${DAHAKA_MODULE_ROOT}/dahaka-tree.h" # ixx"
)


add_library(dahaka INTERFACE ${dahaka_module})

# target_include_directories(dahaka_name} INTERFACE
#   $<BUILD_INTERFACE:${INCLUDE_ROOT}>
#   $<INSTALL_INTERFACE:include>
#   )

# target_sources(dahaka PUBLIC #INTERFACE
#   FILE_SET interface_headers
#   TYPE HEADERS
#   BASE_DIRS ${PULSE_MODULE_ROOT}
#   FILES ${dahaka_module}
#   )
  
# if(MSVC)
#   intelisense_modules(dahaka ${DAHAKA_MODULE_ROOT})
# endif()
