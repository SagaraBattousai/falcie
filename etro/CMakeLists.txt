
set(src_files "matrix.c")
file(GLOB public_headder_files "${INCLUDE_ROOT}/etro/*.h")
file(GLOB private_headder_files *.h)

add_library(etro SHARED
  ${src_files}
  ${public_headder_files}
  ${private_headder_files})


if(MSVC)
  target_compile_options(etro PUBLIC "/arch:AVX512")
endif()

target_include_directories(etro PUBLIC ${INCLUDE_ROOT})


