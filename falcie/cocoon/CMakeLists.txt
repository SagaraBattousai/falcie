
add_executable(cocoon cocoon.cpp)

target_link_libraries(cocoon PUBLIC cactuar)

if(WIN32)
  copy_required_dlls(cocoon)
endif()


target_compile_options(cocoon PRIVATE ${FALCIE_COMPILE_OPTIONS})
