
set(c_sources
  "crypto.c" "encoding.c" "cactuar.c" "cactuar_math.c")

set(cpp_sources "endianness_check.cpp")


set(public_header_files 
  "${PULSE_INCLUDE_ROOT}/crypto.h"
  "${PULSE_INCLUDE_ROOT}/encoding.h"
  "${PULSE_INCLUDE_ROOT}/cactuar.h"
  )
set(private_header_files "endianness_check.h")

add_library(cactuar OBJECT ${c_sources} ${cpp_sources})

target_sources(cactuar PUBLIC FILE_SET public_headers TYPE HEADERS
  BASE_DIRS "${INCLUDE_ROOT}"
  FILES ${public_header_files})

target_sources(cactuar PRIVATE FILE_SET private_headers TYPE HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${private_header_files})

target_compile_definitions(cactuar PRIVATE 
  "OPENSSL_API_COMPAT=30000" 
  "OPENSSL_NO_DEPRECATED")

target_include_directories(cactuar PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(cactuar PRIVATE OpenSSL::Crypto)

#POTENTIAL_ISSUE: -> OpenSSL: Cant find applink :O Maybe because precomiled?
#Sure it's neccisary?
#if(MSVC)
  #target_link_libraries(etro PRIVATE OpenSSL::applink)
#endif()
