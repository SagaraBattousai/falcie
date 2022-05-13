
set(c_sources "matrix.c" "matrix_avx.c" "anima.c" "linalg.c")

add_library(anima OBJECT ${c_sources})
if(x86)
  if(MSVC)
    #For matrix mult
    # target_compile_options(anima PUBLIC "/arch:AVX512")
    target_compile_options(anima PRIVATE "/arch:AVX512")
  else()
    #Risky as I've only check it's accaptable for GCC and Clang.
    # Do I need the specific -mavx512 extension versions?
    target_compile_options(anima PRIVATE "-mavx2")
  endif()
  # I can't trust that __AVX__ is defined. (Or can I?).
  # Leave PUBLIC in case client program wants to test it
  target_compile_definitions(anima PUBLIC USE_AVX_INTRIN)
endif()

target_sources(anima PUBLIC 
  FILE_SET public_headers 
  TYPE HEADERS
  BASE_DIRS "${INCLUDE_ROOT}" 
  FILES 
  "${PULSE_INCLUDE_ROOT}/matrix.h"
  "${PULSE_INCLUDE_ROOT}/anima.h"
  "${PULSE_INCLUDE_ROOT}/linalg.h"
  )
