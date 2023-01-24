cmake_minimum_required(VERSION 3.23)

project(falcie
        VERSION 0.0.2
        DESCRIPTION ""
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Force is just for now until all kinks are worked out
# set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR} CACHE PATH 
  # "Path to the root directory of an OpenSSL installation.")
# find_package(OpenSSL 3 REQUIRED)

set(CMAKE_MODULE_PATH 
  "${PROJECT_SOURCE_DIR}/cmake"
  ${CMAKE_MODULE_PATH}
  )

include(SystemCheck)
include(LibUtils)
# include(ModuleUtils)
include(PythonUtils)

set_system_check_vars()

if (MSVC)
  #Although no working currently for module based dll's it could still
  #Work for the C wrapper, aka etro. TODO: Let's see!
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  # warning level 4 and all warnings as errors
  #Give Wall a try and see if the ones that are turned off are okay
  add_compile_options(/W4) # /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Wno-comment) # -Werror)
endif()

set(BUILD_DOCS ON CACHE BOOL "Whether or not to build documentation.")
set(BUILD_TESTS ON CACHE BOOL "Whether or not to build tests.")

# set(MODULE_ROOT "${PROJECT_SOURCE_DIR}/modules")
set(MODULE_ROOT "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(falcie)

if(BUILD_DOCS)
  add_subdirectory(docs)
  #Do I need to add pulse's dependancies as well or is that transitive!?
  #TODO: # add_dependencies(Doxygen pulse) #lindzei #etro
endif()

# if(MSVC)
#   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT cocoon)
# endif()

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

#TODO: setup installing.....

# install(TARGETS pulse
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# install(TARGETS cactuar
#   FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#if(BUILD_DOCS)
#  #Use trailing slashes to avoid adding subdirectories
#  install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs/build/sphinx/"
#    DESTINATION ${CMAKE_INSTALL_DOCDIR})

#  install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs/build/man/"
#    DESTINATION ${CMAKE_INSTALL_MANDIR})
#endif()

