

add_executable(cocoon cocoon.cpp)

target_link_libraries(cocoon PUBLIC pulse lindzei)

if(WIN32)
  copy_openssl_runtime_lib_to_target(cocoon crypto)
endif()
