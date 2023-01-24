

add_executable(cocoon cocoon.cpp)

# TODO: Remove pulse
target_link_libraries(cocoon PUBLIC pulse lindzei)

if(WIN32)
  symlink_library_to_target(cocoon pulse)
  symlink_library_to_target(cocoon cactuar)
  symlink_library_to_target(cocoon lindzei)
endif()
