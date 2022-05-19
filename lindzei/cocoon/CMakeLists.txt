

add_executable(cocoon cocoon.c)

target_link_libraries(cocoon PUBLIC pulse orphan)

if(WIN32)
  symlink_library_to_target(cocoon pulse)
  symlink_library_to_target(cocoon orphan)
  copy_openssl_runtime_lib_to_target(cocoon crypto)
endif()
