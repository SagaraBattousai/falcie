
set(src_files "matrix.c")

file(GLOB public_headder_files "${INCLUDE_ROOT}/etro/*.h")

add_library(anima OBJECT
  ${src_files}
  ${public_headder_files})

if(MSVC)
  #For matrix mult
  target_compile_options(anima PUBLIC "/arch:AVX512")
endif()

target_include_directories(anima PUBLIC ${INCLUDE_ROOT})
