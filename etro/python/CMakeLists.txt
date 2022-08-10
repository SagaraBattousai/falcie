find_package(Python3 COMPONENTS Development NumPy REQUIRED)

set(FALPY_MODULE_ROOT "${ETRO_MODULE_ROOT}/python/falpy")

set(sources "falpy.c" "tf_model_matrix.c")

set(falpy_module 
  "${FALPY_MODULE_ROOT}/_falpyconfig.h"
  "${FALPY_MODULE_ROOT}/falpy.h" # ixx"
  "${FALPY_MODULE_ROOT}/tf_model_matrix.h" # ixx"
  )

add_library(falpy SHARED ${sources})

as_python_extension(falpy)

target_sources(falpy PRIVATE
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS "${ETRO_MODULE_ROOT}/python"
  FILES ${falpy_module} 
  )


target_include_directories(falpy PUBLIC
  ${Python3_INCLUDE_DIRS}
  ${Python3_NumPy_INCLUDE_DIRS}
  )

target_link_libraries(falpy ${Python3_LIBRARIES} pulse)

if (MSVC)
  set_property(TARGET falpy PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS OFF)
  target_compile_definitions(falpy PRIVATE FALPY_BUILD_DLL)
endif()






