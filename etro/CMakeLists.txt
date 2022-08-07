
set(ETRO_MODULE_ROOT "${MODULE_ROOT}/etro")

add_subdirectory(python)

add_executable(etro_c "etro.c")
add_executable(etro_cpp "etro.cpp")

################################################

set(sources "matrix.cpp")

set(etro_module 
  "${ETRO_MODULE_ROOT}/matrix.h"
  )

add_library(etro SHARED ${sources})

target_sources(etro PUBLIC
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${etro_module}
  )

target_link_libraries(etro PUBLIC pulse lindzei)

target_link_libraries(etro_c PUBLIC pulse)
target_link_libraries(etro_cpp PUBLIC pulse)

if(WIN32)
  symlink_library_to_target(etro_c pulse)
  symlink_library_to_target(etro_cpp pulse)
  copy_openssl_runtime_lib_to_target(etro_c crypto)
  copy_openssl_runtime_lib_to_target(etro_cpp crypto)
endif()

set_target_properties(etro etro_c etro_cpp falpy PROPERTIES FOLDER Etro)
