find_package(OpenSSL 3 REQUIRED)
set(src_files "matrix.c" 
  "blockchain.cpp" "blockheader.c" "federatedblock.c"
  "crypto.c" "encoding.c" "endianness_check.cpp")
file(GLOB public_headder_files "${INCLUDE_ROOT}/etro/*.h")
file(GLOB private_headder_files *.h)

add_library(etro SHARED
  ${src_files}
  ${public_headder_files}
  ${private_headder_files})


if(MSVC)
  #For matrix mult
  target_compile_options(etro PUBLIC "/arch:AVX512")

  #For openssl TODO: Uncomment Cant find applink :O Maybe because precomiled?
  #target_link_libraries(etro PRIVATE OpenSSL::applink)
endif()

#target_compile_definitions(etro PRIVATE "OPENSSL_API_COMPAT=30002" 
#  "OPENSSL_NO_DEPRECATED")
target_include_directories(etro PUBLIC ${INCLUDE_ROOT} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(etro PUBLIC OpenSSL::Crypto)

add_executable(buh main.cpp ${public_headder_files})
target_include_directories(buh PUBLIC ${INCLUDE_ROOT} ${OPENSSL_INCLUDE_DIR})

target_link_libraries(buh PUBLIC etro OpenSSL::Crypto)
