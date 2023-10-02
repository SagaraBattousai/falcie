
add_subdirectory(cactuar)

add_subdirectory(cocoon)

# add_subdirectory(etro)

add_subdirectory(thoth)

# ExternalProject_Add(thoth
#   GIT_REPOSITORY git@github.com:SagaraBattousai/thoth.git
#   GIT_TAG        origin/master
#   STEP_TARGETS   build
# )

# put thoth's targets in a folder (must be a betetr way)
set_target_properties(concepts crypto ndarray network numeric utility
  PROPERTIES FOLDER Thoth)




if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()


