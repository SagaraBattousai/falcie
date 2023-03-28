
add_subdirectory(anima)

add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(cocoon)

add_subdirectory(dahaka)

add_subdirectory(etro)

# add_subdirectory(gran)

add_subdirectory(orphan)

#Added outside etro as doesnot use etro but is a binding
add_subdirectory(etro/Android/faldroid)


if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()


