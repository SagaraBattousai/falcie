####################################################################
################# Cactuar The Low Level Target #####################
####################################################################

set(target_name cactuar)

set(c_sources "crypto.c" "encoding.c" "math.c" "blockheader.c" "target.c")

# "cactuar")

set(cpp_sources "endianness_check.cpp")

set(headers "${INTERNAL_INCLUDE_ROOT}/${target_name}/crypto.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/encoding.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/target.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/math.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/blockheader.h"
            "${INTERNAL_INCLUDE_ROOT}/${target_name}/endianness_check.h")

add_library(${target_name} OBJECT ${c_sources} ${cpp_sources})

target_sources(${target_name} PRIVATE 
  FILE_SET private_headers 
  TYPE HEADERS
  BASE_DIRS "${INTERNAL_INCLUDE_ROOT}"
  FILES 
  ${headers})

target_sources(${target_name} PUBLIC
  FILE_SET public_headers 
  TYPE HEADERS
  BASE_DIRS "${INCLUDE_ROOT}"
  FILES 
  "${PULSE_INCLUDE_ROOT}/${target_name}.h")

target_compile_definitions(${target_name} PRIVATE 
  "OPENSSL_API_COMPAT=30000" 
  "OPENSSL_NO_DEPRECATED")

target_include_directories(${target_name} PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(${target_name} PRIVATE OpenSSL::Crypto)





#POTENTIAL_ISSUE: -> OpenSSL: Cant find applink :O Maybe because precomiled?
#Sure it's neccisary?
#if(MSVC)
  #target_link_libraries(etro PRIVATE OpenSSL::applink)
#endif()
