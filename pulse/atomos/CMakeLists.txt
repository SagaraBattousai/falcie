###################################################################
############### Atomos The Networking Target ######################
###################################################################

set(ATOMOS_MODULE_ROOT "${PULSE_MODULE_ROOT}/atomos")

set(atomos_module 
  "${ATOMOS_MODULE_ROOT}/atomos.ixx"
  "${ATOMOS_MODULE_ROOT}/atomos-network.ixx" 
  # "${ATOMOS_MODULE_ROOT}/anima-neural_network.ixx"
  # "${ATOMOS_MODULE_ROOT}/anima-random.ixx" 
  )

set(cpp_files 
  "atomos-network_unix.cpp" 
  "atomos-network_win32.cpp" 
  ) 

#Can't be Object anymore as add_custom_command for bost build wont work.
#Static as we want pulse to be the dll and we can statically link the dll to 
#this library and then only link to dll from client.
add_library(atomos STATIC ${atomos_module} ${cpp_files})

if(MSVC)
  intelisense_modules(atomos ${ATOMOS_MODULE_ROOT})
endif()

if(WIN32)
  target_link_libraries(atomos PRIVATE Ws2_32)
endif()

