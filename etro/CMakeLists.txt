find_package(OpenSSL 3 REQUIRED)
set(src_files1 "matrix.c" 
  "blockchain.cpp" "federated_blockchain.cpp" 
  "blockheader.c" "federatedblock.c")

set(src_files2
  "crypto.c" "encoding.c" "endianness_check.cpp"
  "cactuar.c" "math.c")

file(GLOB public_headder_files "${INCLUDE_ROOT}/etro/*.h")
file(GLOB private_headder_files *.h)

add_library(etro SHARED
  ${src_files1}
  ${public_headder_files}
  ${private_headder_files})

add_library(pulse OBJECT
  ${src_files2}
  ${public_headder_files}
  ${private_headder_files})


if(MSVC)
  #For matrix mult
  target_compile_options(etro PUBLIC "/arch:AVX512")

  #For openssl TODO?: Uncomment Cant find applink :O Maybe because precomiled?
  #target_link_libraries(etro PRIVATE OpenSSL::applink)
endif()

target_compile_definitions(etro PRIVATE 
  "OPENSSL_API_COMPAT=30000" 
  "OPENSSL_NO_DEPRECATED")

target_include_directories(etro PUBLIC ${INCLUDE_ROOT} ${OPENSSL_INCLUDE_DIR})
target_include_directories(pulse PUBLIC ${INCLUDE_ROOT} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(etro PUBLIC OpenSSL::Crypto pulse)

add_executable(buh main.cpp ${public_headder_files})
target_include_directories(buh PUBLIC ${INCLUDE_ROOT} ${OPENSSL_INCLUDE_DIR})

target_link_libraries(buh PUBLIC etro OpenSSL::Crypto)
