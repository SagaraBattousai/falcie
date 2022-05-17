
set(PULSE_INCLUDE_ROOT "${INCLUDE_ROOT}/pulse")
set(INTERNAL_INCLUDE_ROOT "${PULSE_INCLUDE_ROOT}/internal")

add_subdirectory(anima)

#add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(dahaka)


add_library(pulse SHARED $<TARGET_OBJECTS:anima> $<TARGET_OBJECTS:cactuar>)


target_sources(pulse PUBLIC FILE_SET public_headers TYPE HEADERS 
  BASE_DIRS "${INCLUDE_ROOT}"
  FILES 
  "${PULSE_INCLUDE_ROOT}/anima.h"
  "${PULSE_INCLUDE_ROOT}/cactuar.h"
  "${PULSE_INCLUDE_ROOT}/dahaka.h"
)

target_link_libraries(pulse PUBLIC anima cactuar dahaka)

target_include_directories(pulse PUBLIC "${INCLUDE_ROOT}")
target_include_directories(pulse PRIVATE "${INTERNAL_INCLUDE_ROOT}")

set_target_properties(pulse anima cactuar dahaka PROPERTIES FOLDER Pulse)







