
## TFLITE PACKAGE

set(TENSORFLOW_SOURCE_DIR "" CACHE PATH
  "Directory that contains the TensorFlow project" )
if(NOT TENSORFLOW_SOURCE_DIR)
  get_filename_component(TENSORFLOW_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/tensorflow_src" ABSOLUTE)
endif()

add_subdirectory(
  "${TENSORFLOW_SOURCE_DIR}/tensorflow/lite"
  "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite" EXCLUDE_FROM_ALL)


if(MSVC)
  #Ensure export all only for tflite? TODO: TEMP:
  set_property(TARGET tensorflow-lite PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_subdirectory(lib)

target_link_libraries(tensorflow-lite PUBLIC tensorflowlite_flex)

add_subdirectory(valhalla)
add_subdirectory(app)
