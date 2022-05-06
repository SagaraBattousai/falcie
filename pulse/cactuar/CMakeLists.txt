
set(src_files
  "crypto.c" "encoding.c" "endianness_check.cpp"
  "cactuar.c" "math.c")

file(GLOB public_headder_files "${INCLUDE_ROOT}/etro/*.h")
file(GLOB private_headder_files "*.h")

add_library(cactuar OBJECT
  ${src_files}
  ${public_headder_files}
  ${private_headder_files})


#OpenSSL: Cant find applink :O Maybe because precomiled? Sure it's neccisary?
#if(MSVC)
  #target_link_libraries(etro PRIVATE OpenSSL::applink)
#endif()

target_compile_definitions(cactuar PRIVATE 
  "OPENSSL_API_COMPAT=30000" 
  "OPENSSL_NO_DEPRECATED")

target_include_directories(cactuar PUBLIC 
  ${INCLUDE_ROOT} 
  ${OPENSSL_INCLUDE_DIR})

target_link_libraries(cactuar PUBLIC OpenSSL::Crypto)
