###################################################################
################## Anima The AI Target ############################
###################################################################

set(target_name anima)

set(c_sources "matrix.c" "linalg.c" "matrix_avx.c" "ai.c")

set(headers "${INTERNAL_INCLUDE_ROOT}/${target_name}/matrix.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/linalg.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/ai.h"
            )

add_library(${target_name} OBJECT ${c_sources})

target_sources(${target_name} PRIVATE
  FILE_SET private_headers 
  TYPE HEADERS
  BASE_DIRS "${INTERNAL_INCLUDE_ROOT}" 
  FILES
  ${headers}
  )

target_sources(${target_name} PUBLIC
  FILE_SET public_headers 
  TYPE HEADERS
  BASE_DIRS "${INCLUDE_ROOT}"
  FILES 
  "${PULSE_INCLUDE_ROOT}/${target_name}.h")

if(x86)
  if(MSVC)
    #For matrix mult
    target_compile_options(${target_name} PRIVATE "/arch:AVX512")
  else()
    #Risky as I've only check it's accaptable for GCC and Clang.
    # Do I need the specific -mavx512 extension versions?
    target_compile_options(${target_name} PRIVATE "-mavx2")
  endif()
  # I can't trust that __AVX__ is defined. (Or can I?).
  # Leave PUBLIC in case client program wants to test it
  target_compile_definitions(${target_name} PUBLIC USE_AVX_INTRIN)
endif()

