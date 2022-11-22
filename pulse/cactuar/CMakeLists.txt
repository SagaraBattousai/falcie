####################################################################
################# Cactuar The Low Level Target #####################
####################################################################

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(CACTUAR_MODULE_ROOT "${PULSE_MODULE_ROOT}/cactuar")
set(CACTUAR_INTERNAL_MODULE_ROOT "${PULSE_MODULE_ROOT}/cactuar/internal")

set(cpp_sources 
  "block.cpp"
  "blockheader.cpp"
  "crypto.cpp"
  "encoding.cpp" 
  "functional.cpp"
  "hash.cpp" 
  "hash_pool.cpp"
  "openssl_wrapper.cpp"
  "signing.cpp" 
  "target.cpp"
  "transaction.cpp"
  )

set(cactuar_module 
  "${CACTUAR_MODULE_ROOT}/cactuar.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-block.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-blockheader.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-crypto.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-functional.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-hash.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-hash_pool.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-signing.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-encoding.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-target.h" # ixx"
  "${CACTUAR_MODULE_ROOT}/cactuar-transaction.h" # ixx"
  )

set(cactuar_private_headers
  "${CACTUAR_INTERNAL_MODULE_ROOT}/cactuar-openssl_wrapper.h"
  )

add_library(cactuar OBJECT ${cpp_sources})

target_sources(cactuar PUBLIC
  FILE_SET headers 
  TYPE HEADERS
  BASE_DIRS ${PULSE_MODULE_ROOT}
  FILES ${cactuar_module}
  )

target_sources(cactuar PRIVATE
  FILE_SET internal_headers 
  TYPE HEADERS
  BASE_DIRS ${CACTUAR_INTERNAL_MODULE_ROOT}
  FILES ${cactuar_private_headers}
  )

target_compile_definitions(cactuar PRIVATE 
  "OPENSSL_API_COMPAT=30000" 
  "OPENSSL_NO_DEPRECATED")

target_include_directories(cactuar PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(cactuar PRIVATE OpenSSL::Crypto)

# if(MSVC)
#   intelisense_modules(cactuar ${CACTUAR_MODULE_ROOT})
# endif()

# Can't work with static and therefore modules.
if(MSVC)
  target_link_libraries(cactuar PRIVATE OpenSSL::applink)
endif()

