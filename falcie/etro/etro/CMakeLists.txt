
set(ETRO_MODULE_ROOT "${MODULE_ROOT}/etro")
set(ETRO_FOLDER_NAME "Etro")

add_executable(etro_c "etro.c")
add_executable(etro_cpp "etro.cpp")

################################################

set(sources 
  "matrix.cpp"
  "block.cpp"
  "blockchain.cpp"
  "networkupdate.cpp"
  )

set(etro_module 
  "${ETRO_MODULE_ROOT}/matrix.h"
  "${ETRO_MODULE_ROOT}/block.h"
  "${ETRO_MODULE_ROOT}/blockchain.h"
  "${ETRO_MODULE_ROOT}/networkupdate.h"
  )

add_library(etro SHARED ${sources})

target_sources(etro PUBLIC
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${etro_module}
  )

target_link_libraries(etro PUBLIC cactuar)

target_link_libraries(etro_c PUBLIC etro)
target_link_libraries(etro_cpp PUBLIC etro)

if(WIN32)
  copy_required_dlls(etro_c)
  copy_required_dlls(etro_cpp)
  # copy_openssl_runtime_lib_to_target(etro_c crypto)
  # copy_openssl_runtime_lib_to_target(etro_cpp crypto)
endif()


set_target_properties(etro etro_c etro_cpp PROPERTIES FOLDER ${ETRO_FOLDER_NAME})


#TODO: MAKE ETRO MORE......C++ as well as C Happy!!!!
#OR Not, Just move the add_subdir call out one


#TODO: Maybe
# set(BUILD_PYTHON_BINDINGS ON CACHE BOOL 
#  "Whether or not to build Python bindings")

