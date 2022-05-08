
set(c_sources "matrix.c")

add_library(anima OBJECT ${c_sources})

if(MSVC)
  #For matrix mult
  target_compile_options(anima PUBLIC "/arch:AVX512")
endif()

target_sources(anima PUBLIC 
  FILE_SET public_headers 
  TYPE HEADERS
  BASE_DIRS "${INCLUDE_ROOT}" 
  FILES "${PULSE_INCLUDE_ROOT}/matrix.h")
