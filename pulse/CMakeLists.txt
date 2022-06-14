
set(PULSE_MODULE_ROOT "${MODULE_ROOT}/pulse")
# set(INTERNAL_INCLUDE_ROOT "${PULSE_INCLUDE_ROOT}/internal")

add_subdirectory(anima)

add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(dahaka)

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(pulse_module "${PULSE_MODULE_ROOT}/pulse.h") # ixx")

#Currently CMake Can't handle making a dll with C++20 Modules and 
#setting CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS; therfore, for now, we 
#shall make it static.
add_library(pulse SHARED #STATIC  
  ${pulse_module}
  $<TARGET_OBJECTS:anima>
  $<TARGET_OBJECTS:atomos>
  $<TARGET_OBJECTS:cactuar>
  )


target_link_libraries(pulse PUBLIC anima atomos cactuar dahaka)

set_target_properties(pulse anima atomos cactuar dahaka PROPERTIES FOLDER Pulse)

target_sources(pulse PUBLIC
  FILE_SET headers 
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${pulse_module}
  )

# if(MSVC)
#   intelisense_modules(pulse ${PULSE_MODULE_ROOT})
# endif()





