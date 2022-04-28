
macro(configure_sphinx_and_add_command)

  set(SPHINX_CSS_FILES_LIST "\"css/style.css\",")

  set(SPHINX_JS_FILES_LIST "") #"\"js/falcie_docs.js\","

  set(BREATHE_DOMAIN_BY_EXTENSION_DICT "\"h\" : \"c\",")

  set(BREATHE_DOMAIN_BY_FILE_PATTERN_DICT 
  "\"../include/etro/chain.h\" : \"cpp\",
   \"../include/etro/lish.h\" : \"cpp\",
   \"../include/etro/blockchain.h\" : \"cpp\",")

  configure_file(${SPHINX_CONF_IN} ${SPHINX_CONF_OUT} @ONLY)

  # As this is a macro, this will be the directory of the caller! :)
  set(SPHINX_DOXYGEN_XML_DIR 
    "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}/xml")

  file(GLOB_RECURSE SPHINX_RESTRUCTRED_TEXT "${SPHINX_ROOT_DIRECTORY}/*.rst")

  #VV Could one day replace globs since files (tho escaped) are manually listed
  #VV above ....
  set(SPHINX_STATIC_DIR "${SPHINX_ROOT_DIRECTORY}/_static")
  file(GLOB_RECURSE SPHINX_CSS "${SPHINX_STATIC_DIR}/css/*.css")
  file(GLOB_RECURSE SPHINX_JS "${SPHINX_STATIC_DIR}/js/*.js")

  add_custom_command(OUTPUT ${SPHINX_HTML_INDEX_FILE}
    COMMAND
    ${SPHINX_EXECUTABLE} 
    -b html
    -j auto
    "-Dbreathe_projects.${PROJECT_NAME}=${SPHINX_DOXYGEN_XML_DIR}"
    "-Dbreathe_default_project=${PROJECT_NAME}"
    ${SPHINX_ROOT_DIRECTORY} ${SPHINX_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    MAIN_DEPENDENCY ${SPHINX_CONF_OUT}
    DEPENDS ${SPHINX_RESTRUCTRED_TEXT} ${DOXYGEN_XML_INDEX_FILE}
    ${SPHINX_CSS} ${SPHINX_JS}
    COMMENT "Generating documentation with Sphinx")

endmacro()
