cmake_minimum_required(VERSION 3.22)


project(falcie
        VERSION 0.0.1
        DESCRIPTION ""
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Force is just for now until all kinks are worked out
set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR} CACHE PATH 
  "Path to the root directory of an OpenSSL installation.")
find_package(OpenSSL 3 REQUIRED)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(LibUtils)





if(MSVC)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(BUILD_DOCS ON CACHE BOOL "Whether or not to build documentation")

set(INCLUDE_ROOT "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(pulse)
#add_subdirectory(etro)

# add_library(etro SHARED) # $<TARGET_OBJECTS:pulse>)



add_executable(T "etro/main.cpp")
target_link_libraries(T pulse)
target_include_directories(T PUBLIC ${INCLUDE_ROOT})
copy_library_to_target(T pulse)
copy_openssl_runtime_lib_to_target(T crypto)

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()











