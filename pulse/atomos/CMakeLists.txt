###################################################################
############### Atomos The Networking Target ######################
###################################################################

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(ATOMOS_MODULE_ROOT "${PULSE_MODULE_ROOT}/atomos")

set(atomos_module 
  "${ATOMOS_MODULE_ROOT}/atomos.h" #ixx"
  "${ATOMOS_MODULE_ROOT}/atomos-network.h" #ixx" 
  "${ATOMOS_MODULE_ROOT}/atomos-network_win32.h" #ixx" 
  "${ATOMOS_MODULE_ROOT}/atomos-network_unix.h" #ixx" 
  )

set(cpp_files 
  "atomos-network.cpp" 
  "atomos-network_unix.cpp" 
  "atomos-network_win32.cpp" 
  ) 

add_library(atomos OBJECT ${atomos_module} ${cpp_files})

target_sources(atomos PUBLIC
  FILE_SET headers 
  TYPE HEADERS
  BASE_DIRS ${PULSE_MODULE_ROOT}
  FILES ${atomos_module}
  )

if(MSVC)
  # intelisense_modules(atomos ${ATOMOS_MODULE_ROOT})
  target_link_libraries(atomos PRIVATE Ws2_32)
endif()

