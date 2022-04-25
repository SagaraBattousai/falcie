find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

###################################################################
##### DOCUMENTATION ROOT PATHS AND CONFIG FILE VARIABLES ##########
###################################################################

set(DOXYFILE_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doxygen")
set(SPHINX_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/sphinx")

set(DOXYFILE_IN "${DOXYFILE_ROOT_DIRECTORY}/Doxyfile.in")
set(DOXYFILE_OUT "${DOXYFILE_ROOT_DIRECTORY}/Doxyfile")

set(SPHINX_CONF_IN "${SPHINX_ROOT_DIRECTORY}/conf.py.in")
set(SPHINX_CONF_OUT "${SPHINX_ROOT_DIRECTORY}/conf.py")



###############################################
##### DOXYGEN VARIABLES AND COMMAND SETUP #####
###############################################

file(GLOB_RECURSE HEADER_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  ${INCLUDE_ROOT}/*.h )

string(JOIN " " DOXYGEN_INPUT_FILES ${HEADER_FILES})
set(DOXYGEN_OUTPUT_DIRECTORY "build")
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_LATEX NO)
set(DOXYGEN_JAVADOC_BANNER YES)
set(DOXYGEN_TAB_SIZE 2) #Default is 4

###########################################################
#### CONFIGURE DOXYFILE AND ADD DOXYGEN CUSTOM COMMAND ####
###########################################################

#Can't use doxygen_add_docs as we need doxyfile to be usable for read-the-docs.
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

set(DOXYGEN_XML_INDEX_FILE 
  "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}/xml/index.xml")

add_custom_command(OUTPUT ${DOXYGEN_XML_INDEX_FILE}
  DEPENDS ${HEADER_FILES} ${DOXYFILE_IN}
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  MAIN_DEPENDENCY ${DOXYFILE_OUT}
  #  #^^ I don't really get what its for in vs, what does hang mean?
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating Doxygen Documentation ...")

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_XML_INDEX_FILE})


# Relative version is for configuring the file as required for read-the-docs 
set(SPHINX_RELATIVE_DOXYGEN_XML_DIR "../${DOXYGEN_OUTPUT_DIRECTORY}/xml")

configure_file(${SPHINX_CONF_IN} ${SPHINX_CONF_OUT} @ONLY)


set(SPHINX_BUILD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/sphinx")
set(SPHINX_HTML_INDEX_FILE "${SPHINX_BUILD_DIRECTORY}/index.html")

set(SPHINX_DOXYGEN_XML_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}/xml")


add_custom_command(OUTPUT ${SPHINX_HTML_INDEX_FILE}
  COMMAND
  ${SPHINX_EXECUTABLE} 
  -b html
  -j auto
  "-Dbreathe_projects.${PROJECT_NAME}=${SPHINX_DOXYGEN_XML_DIR}"
  "-Dbreathe_default_project=${PROJECT_NAME}"
  ${SPHINX_ROOT_DIRECTORY} ${SPHINX_BUILD_DIRECTORY}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  MAIN_DEPENDENCY ${SPHINX_CONF_OUT}
  DEPENDS "${SPHINX_ROOT_DIRECTORY}/index.rst" ${DOXYGEN_XML_INDEX_FILE}
  COMMENT "Generating documentation with Sphinx")


add_custom_target(Sphinx ALL DEPENDS ${SPHINX_HTML_INDEX_FILE})

