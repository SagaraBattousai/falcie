
add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(cocoon)

# add_subdirectory(etro)

add_subdirectory(orphan)

add_subdirectory(thoth)

add_subdirectory(faldroid/faldroid)


if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()


