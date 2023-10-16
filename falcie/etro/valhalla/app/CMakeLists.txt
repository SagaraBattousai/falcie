
add_executable(app_cpp_valhalla main.cpp)

target_link_libraries(app_cpp_valhalla valhalla)

auto_copy_dlls_for_target(app_cpp_valhalla)
