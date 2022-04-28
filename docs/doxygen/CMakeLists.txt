

# The following Variables must be set: 
# { DOXYGEN_INPUT_FILES, DOXYGEN_OUTPUT_DIRECTORY, DOXYGEN_XML_INDEX_FILE,
#   DOXYFILE_IN, DOXYFILE_OUT }

macro(configure_doxygen_and_add_command)
  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_GENERATE_HTML NO)
  set(DOXYGEN_GENERATE_LATEX NO)
  set(DOXYGEN_JAVADOC_BANNER YES)

  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

  #Can't use doxygen_add_docs as we need doxyfile to be usable for read-the-docs.
  add_custom_command(OUTPUT ${DOXYGEN_XML_INDEX_FILE}
    DEPENDS ${HEADER_FILES} ${DOXYFILE_IN}
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
    MAIN_DEPENDENCY ${DOXYFILE_OUT}
    #^^ I don't really get what its for in vs, what does hang mean?
    #VV As this is a macro, this will be the directory of the caller! :)
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating Doxygen Documentation ...")
endmacro()

# INCLUDE_ROOT variable must be set 
macro(set_doxygen_input_files)

  file(GLOB_RECURSE HEADER_FILES 
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${INCLUDE_ROOT}/*.h )

  file(GLOB SOURCE_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/etro/*.c 
    ${PROJECT_SOURCE_DIR}/etro/*.cpp)

  string(JOIN " " DOXYGEN_INPUT_FILES ${HEADER_FILES} ${SOURCE_FILES})
endmacro()
