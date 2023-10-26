
add_executable(cocoon cocoon.cpp)
target_compile_options(cocoon PRIVATE ${FALCIE_COMPILE_OPTIONS})

target_link_libraries(cocoon PUBLIC cactuar)

if(WIN32)
  # symlink_library_to_target(cocoon cactuar)
endif()
