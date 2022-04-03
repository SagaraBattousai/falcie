cmake_minimum_required(VERSION 3.18)

project(falcie
        VERSION 0.0.1
        DESCRIPTION ""
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(INCLUDE_ROOT "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(etro)
