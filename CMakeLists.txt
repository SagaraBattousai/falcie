cmake_minimum_required(VERSION 3.26)

project(falcie
  VERSION 0.0.3
  DESCRIPTION ""
  LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH 
  "${PROJECT_SOURCE_DIR}/cmake"
  ${CMAKE_MODULE_PATH}
  )

# include(ExternalProject)
include(SystemCheck)
include(LibUtils)
include(PythonUtils) # May move to etro

set_system_check_vars()

if (MSVC)
  add_compile_options(/W4 /WX /permissive /wd4100 /wd4251)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-comment -Wno-unknown-pragmas -Wno-unused-parameter)
endif()

option(FALCIE_BUILD_DOCS "Whether or not to build documentation.")
option(FALCIE_BUILD_TESTS "Whether or not to build tests.")

option(GENERATE_PYTHON_BINDINGS "Whether or not to add the python bindings target (falpy).")
option(GENERATE_TENSORFLOWLITE_BINDINGS "Whether or not to add the tensorflow lite bindings target (valhalla).")

set(MODULE_ROOT "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(falcie)

if(FALCIE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(FALCIE_BUILD_DOCS)
  add_subdirectory(docs)
  #Do I need to add pulse's dependancies as well or is that transitive!?
  # add_dependencies(Doxygen <Target_dependancies>)
endif()


# if(MSVC)
#   set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT cocoon)
# endif()

#----------------------------------------------
#   Install settings
#----------------------------------------------

include(GNUInstallDirs)

set(IN_BUILD_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")

option(INSTALL_IN_BUILD
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

