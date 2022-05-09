
add_library(dahaka INTERFACE
  "${PULSE_INCLUDE_ROOT}/chain.h"
  "${PULSE_INCLUDE_ROOT}/blockchain.h"
  "${PULSE_INCLUDE_ROOT}/orderedlist.h"
  )

#target_sources(dahaka PUBLIC FILE_SET public_headers TYPE HEADERS
#  BASE_DIRS "${INCLUDE_ROOT}" FILES ${public_headder_files})

target_include_directories(dahaka INTERFACE
  $<BUILD_INTERFACE:${INCLUDE_ROOT}>
  $<INSTALL_INTERFACE:include>
  )
