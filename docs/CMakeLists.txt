find_package(Doxygen REQUIRED)

file(GLOB_RECURSE HEADER_FILES ${INCLUDE_ROOT}/*.h)

# string(JOIN " " DOXYGEN_INPUT_FILES ${HEADER_FILES})
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_LATEX NO)
set(DOXYGEN_JAVADOC_BANNER YES)
set(DOXYGEN_TAB_SIZE 2) #Default is 4

set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile") #Change to c if d

#Can't use doxygen_add_docs as we need doxyfile to be usable for read-the-docs.

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

set(DOXYGEN_OUTPUT_INDEX "${DOXYGEN_OUTPUT_DIRECTORY}/xml/index.xml")
add_custom_command(OUTPUT ${DOXYGEN_OUTPUT_INDEX}
  DEPENDS ${DOXYGEN_INPUT_FILES} ${DOXYFILE_OUT}
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  MAIN_DEPENDENCY ${DOXYFILE_IN} # Out could go here but should only have 1 main imo
#  #^^ Also don't really get what its for in vs, what does hang mean?
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating Doxygen Documentation ...")

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_OUTPUT_INDEX})

find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/sphinx)
set(SPHINX_BUILD ${CMAKE_CURRENT_SOURCE_DIR}/build/sphinx)

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
