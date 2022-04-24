find_package(Doxygen REQUIRED)

# message(STATUS "Exists ${INCLUDE_ROOT}")

file(GLOB_RECURSE HEADER_FILES ${INCLUDE_ROOT}/*.h)

set(DOXYGEN_INDEX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/html/index.html)

add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
  COMMAND Doxygen::doxygen Doxyfile
  MAIN_DEPENDENCY Doxyfile
  DEPENDS ${HEADER_FILES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Old form to Generate docs")

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})



