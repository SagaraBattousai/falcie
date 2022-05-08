cmake_minimum_required(VERSION 3.23)

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

#DLL's must be in same dir as exe (or added to path but i'd rather
#avoid that if I can.
if(WIN32)
  copy_library_to_target(T pulse)
  copy_openssl_runtime_lib_to_target(T crypto)
endif()

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()

#----------------------------------------------
#   Install settings
#----------------------------------------------

include(GNUInstallDirs)

set(IN_BUILD_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

set(INSTALL_IN_BUILD OFF CACHE BOOL 
  "If set, the CMAKE_INSTALL_PREFIX will be set to the build directory\
  (unless CMAKE_INSTALL_PREFIX has been set by the user).\nIn this case\
  CMAKE_INSTALL_PREFIX = ${IN_BUILD_INSTALL_PREFIX}")

if(INSTALL_IN_BUILD AND NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX 
    "${IN_BUILD_INSTALL_PREFIX}"
    CACHE PATH "..." FORCE)
endif()


#TEST
# get_property(TP TARGET cactuar PROPERTY LINK_LIBRARIES)
# message(STATUS "......**** ${TP}")
# get_property(TP TARGET cactuar PROPERTY INCLUDE_DIRECTORIES)
# message(STATUS "......**** ${TP}")




#TODO: setup installing.....



install(TARGETS pulse
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# install(TARGETS cactuar
#   FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(IMPORTED_RUNTIME_ARTIFACTS OpenSSL::Crypto
#   RUNTIME_DEPENDENCY_SET external_runtime_deps RUNTIME)





if(WIN32)
  get_openssl_runtime_lib(crypto)
  install(PROGRAMS ${OPENSSL_RUNTIME_LIB_crypto} 
    DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(BUILD_DOCS)
  #Use trailing slashes to avoid adding subdirectories
  install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs/build/sphinx/"
    DESTINATION ${CMAKE_INSTALL_DOCDIR})

  install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs/build/man/"
    DESTINATION ${CMAKE_INSTALL_MANDIR})
endif()


