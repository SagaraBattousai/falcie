
add_executable(cocoon cocoon.cpp)

target_link_libraries(cocoon PUBLIC cactuar)

if(WIN32)
  # symlink_library_to_target(cocoon cactuar)
endif()
