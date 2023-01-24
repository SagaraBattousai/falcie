

add_executable(gran gran.cpp)

target_link_libraries(gran PUBLIC pulse)
# target_include_directories(gran PRIVATE ${OPENSSL_INCLUDE_DIR})
# target_link_libraries(gran PRIVATE OpenSSL::Crypto)

if(WIN32)
  # copy_openssl_runtime_lib_to_target(gran crypto)
  symlink_library_to_target(gran pulse)
  # symlink_library_to_target(gran lindzei)
  auto_copy_dlls_for_target(gran)
endif()
