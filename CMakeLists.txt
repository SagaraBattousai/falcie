cmake_minimum_required(VERSION 3.22)

project(falcie
        VERSION 0.0.1
        DESCRIPTION ""
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(MSVC)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(BUILD_DOCS ON CACHE BOOL "Whether or not to build documentation")

set(INCLUDE_ROOT "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(etro)

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()

