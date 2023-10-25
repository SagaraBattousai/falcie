
add_executable(app_cpp_valhalla main.cpp)

# target_link_libraries(app_cpp_valhalla valhalla)
target_link_libraries(app_cpp_valhalla PRIVATE tensorflow-lite tensorflowlite_flex)
target_compile_options(app_cpp_valhalla PRIVATE /wd4189 /wd4996)

copy_required_dlls(app_cpp_valhalla)
