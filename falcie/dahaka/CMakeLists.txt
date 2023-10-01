###################################################################
############### Dahaka The Networking Target ######################
###################################################################

set(DAHAKA_MODULE_ROOT "${MODULE_ROOT}/dahaka")

set(dahaka_module 
  "${DAHAKA_MODULE_ROOT}/network.h"
  "${DAHAKA_MODULE_ROOT}/network_win32.h"
  "${DAHAKA_MODULE_ROOT}/network_unix.h"
  )

set(cpp_files 
  "network.cpp" 
  "network_win32.cpp" 
  "network_unix.cpp" 
  ) 

add_library(dahaka SHARED ${dahaka_module} ${cpp_files})

target_sources(dahaka PUBLIC
  FILE_SET headers 
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${dahaka_module}
  "${MODULE_ROOT}/_falcie_config.h"
  )

if(MSVC)
  target_link_libraries(dahaka PRIVATE Ws2_32)
endif()

