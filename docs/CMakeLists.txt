find_package(Doxygen REQUIRED)

file(GLOB_RECURSE HEADER_FILES ${INCLUDE_ROOT}/*.h)

#Sort of has to be in Source for Read-The-Docs but if we change sphinx conf to an in
#we could configure here ....
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/doxygen)
#set(DOXYGEN_GENERATE_MAN YES)
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_JAVADOC_BLOCK YES)

#Dont call DOXYGEN_ as that may be interpreted as a Doxygen option
set(DOXY_TARGET_NAME "Doxygen")
set(DOXYFILE_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.${DOXY_TARGET_NAME}")

doxygen_add_docs(${DOXY_TARGET_NAME}
  ${HEADER_FILES}
  ALL
  USE_STAMP_FILE
  COMMENT "Generating Doxygen Documentation ...")

add_custom_command(TARGET ${DOXY_TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different 
  ${DOXYFILE_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Copying Doxyfile to Docs source for use with read-the-docs etc ...")

find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/sphinx)
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)

add_custom_target(Sphinx ALL
  COMMAND
  ${SPHINX_EXECUTABLE} 
  -b html
  -j auto
  -Dbreathe_projects.falcie="${DOXYGEN_OUTPUT_DIRECTORY}/xml/"
  -Dbreathe_default_project="falcie"
  ${SPHINX_SOURCE} ${SPHINX_BUILD}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating documentation with Sphinx")
