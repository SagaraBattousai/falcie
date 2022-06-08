####################################################################
################# Cactuar The Low Level Target #####################
####################################################################

set(CACTUAR_MODULE_ROOT "${PULSE_MODULE_ROOT}/cactuar")

set(cpp_sources 
  "block.cpp"
  # "blockheader.cpp"
  "crypto.cpp" 
  "encoding.cpp" 
  "target.cpp"
  )

set(cactuar_module 
  "${CACTUAR_MODULE_ROOT}/cactuar.ixx"
  "${CACTUAR_MODULE_ROOT}/block.ixx"
  "${CACTUAR_MODULE_ROOT}/blockheader.ixx"
  "${CACTUAR_MODULE_ROOT}/crypto.ixx"
  "${CACTUAR_MODULE_ROOT}/encoding.ixx"
  "${CACTUAR_MODULE_ROOT}/target.ixx"
  )

add_library(cactuar STATIC ${cpp_sources} ${cactuar_module})

if(MSVC)
  intelisense_modules(cactuar ${CACTUAR_MODULE_ROOT})
endif()

target_compile_definitions(cactuar PRIVATE 
  "OPENSSL_API_COMPAT=30000" 
  "OPENSSL_NO_DEPRECATED")

target_include_directories(cactuar PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(cactuar PRIVATE OpenSSL::Crypto)


if(MSVC)
endif()

 





# Can't work due to static and modules me thinks.
#if(MSVC)
#  target_link_libraries(cactuar PRIVATE OpenSSL::applink)
#endif()

