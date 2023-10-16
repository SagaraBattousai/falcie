
add_subdirectory(cactuar)

add_subdirectory(cocoon)

add_subdirectory(etro)

add_subdirectory(thoth)

# put thoth's targets in a folder (must be a betetr way)
set_target_properties(concepts crypto ndarray network numeric utility
  PROPERTIES FOLDER Thoth)
