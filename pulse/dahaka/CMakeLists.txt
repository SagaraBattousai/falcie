
set(src_files "federated_blockchain.cpp" "blockheader.c" "federatedblock.c")

file(GLOB public_headder_files "${INCLUDE_ROOT}/etro/*.h")
# file(GLOB private_headder_files "*.h")

add_library(dahaka OBJECT
  ${src_files}
  ${public_headder_files})
#  ${private_headder_files})

target_include_directories(dahaka PUBLIC ${INCLUDE_ROOT})

