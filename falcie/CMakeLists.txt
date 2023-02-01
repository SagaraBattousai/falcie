
add_subdirectory(anima)

add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(cocoon)

add_subdirectory(etro)

# add_subdirectory(gran)

add_subdirectory(orphan)


if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()


