
set(ETRO_FOLDER_NAME "Etro")

add_subdirectory(etro)
# put targets in a folder (must be a betetr way)
set_target_properties(etro PROPERTIES FOLDER "${ETRO_FOLDER_NAME}")

if (GENERATE_PYTHON_BINDINGS)
  add_subdirectory(falpy)
set_target_properties(falpy PROPERTIES FOLDER "${ETRO_FOLDER_NAME}")
endif()

if (GENERATE_TENSORFLOWLITE_BINDINGS)
  add_subdirectory(valhalla)
set_target_properties(valhalla PROPERTIES FOLDER "${ETRO_FOLDER_NAME}")
endif()

