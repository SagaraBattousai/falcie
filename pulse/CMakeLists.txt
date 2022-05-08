
set(PULSE_INCLUDE_ROOT "${INCLUDE_ROOT}/pulse")

add_subdirectory(anima)

#add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(dahaka)


add_library(pulse SHARED "blockheader.c" "federatedblock.c" "federated_blockchain.cpp"
  $<TARGET_OBJECTS:anima> $<TARGET_OBJECTS:cactuar>)

target_sources(pulse PUBLIC FILE_SET public_headers TYPE HEADERS 
  BASE_DIRS "${INCLUDE_ROOT}"
  FILES "${PULSE_INCLUDE_ROOT}/types.h"
  "${PULSE_INCLUDE_ROOT}/blockheader.h"
  "${PULSE_INCLUDE_ROOT}/federatedblock.h"
  "${PULSE_INCLUDE_ROOT}/federated_blockchain.h")

target_link_libraries(pulse PUBLIC anima cactuar dahaka)

target_include_directories(pulse PUBLIC "${INCLUDE_ROOT}")




