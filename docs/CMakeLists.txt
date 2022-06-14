find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

set(DOCS_OUTPUT_DIR "build")

set(DOXYFILE_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doxygen")
set(SPHINX_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/sphinx")

add_subdirectory(${DOXYFILE_ROOT_DIRECTORY})
add_subdirectory(${SPHINX_ROOT_DIRECTORY})

set(DOXYFILE_IN "${DOXYFILE_ROOT_DIRECTORY}/Doxyfile.in")
set(DOXYFILE_OUT "${DOXYFILE_ROOT_DIRECTORY}/Doxyfile")

set(SPHINX_CONF_IN "${SPHINX_ROOT_DIRECTORY}/conf.py.in")
set(SPHINX_CONF_OUT "${SPHINX_ROOT_DIRECTORY}/conf.py")

#Must be relative for rtd as this variable is set on Doxyfile.in
set(DOXYGEN_OUTPUT_DIRECTORY ${DOCS_OUTPUT_DIR})
set(DOXYGEN_XML_INDEX_FILE 
  "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}/xml/index.xml")

set(SPHINX_OUTPUT_DIRECTORY 
  "${CMAKE_CURRENT_SOURCE_DIR}/${DOCS_OUTPUT_DIR}/sphinx")
set(SPHINX_HTML_INDEX_FILE "${SPHINX_OUTPUT_DIRECTORY}/index.html")

# Relative version is for configuring the file as required for read-the-docs 
# This is set to be relative to sphinx's conf.py
set(SPHINX_RELATIVE_DOXYGEN_XML_DIR "../${DOXYGEN_OUTPUT_DIRECTORY}/xml")

set_doxygen_input_files(pulse anima cactuar dahaka)
configure_doxygen_and_add_command()
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_XML_INDEX_FILE})

configure_sphinx_and_add_command()
add_custom_target(Sphinx ALL DEPENDS ${SPHINX_HTML_INDEX_FILE})
add_dependencies(Sphinx Doxygen)

set_target_properties(Doxygen Sphinx PROPERTIES FOLDER Docs EXCLUDE_FROM_ALL ON)
