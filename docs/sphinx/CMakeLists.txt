
macro(configure_sphinx_and_add_command)

  set(SPHINX_STATIC_DIR "${SPHINX_ROOT_DIRECTORY}/_static")

  set(SPHINX_CSS 
    "${SPHINX_STATIC_DIR}/css/style.css"
    "${SPHINX_STATIC_DIR}/css/colour.css"
    "${SPHINX_STATIC_DIR}/css/defaults.css"
    "${SPHINX_STATIC_DIR}/css/dark.css"
    "${SPHINX_STATIC_DIR}/css/light.css"
    )

  #Could use list(TRANSFORM but atm its just as easy to write twice
  #Order matters
  set(SPHINX_CSS_FILES_LIST "
  \"css/style.css\",
  \"css/colour.css\",
  \"css/defaults.css\",
  \"css/dark.css\",
  \"css/light.css\",
  ")


  # set(SPHINX_JS_FILES_LIST "") #"\"js/falcie_docs.js\","

  #set(SPHINX_JS "${SPHINX_STATIC_DIR}/js/falcie.js")


  # Now everything should be C++
  set(BREATHE_DOMAIN_BY_EXTENSION_DICT "\"h\" : \"cpp\",")

  #TODO: Once etro (the C wrapper) is up and running
  # set(BREATHE_DOMAIN_BY_FILE_PATTERN_DICT "
  # \"*/include/etro .....\" : \"c\",

  configure_file(${SPHINX_CONF_IN} ${SPHINX_CONF_OUT} @ONLY)

  # As this is a macro, this will be the directory of the caller! :)
  set(SPHINX_DOXYGEN_XML_DIR 
    "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}/xml")

  #Sphinx Top Level rst's
  set(SPHINX_RESTRUCTRED_TEXT 
    "${SPHINX_ROOT_DIRECTORY}/pulse/pulse.rst"
    "${SPHINX_ROOT_DIRECTORY}/pulse/atomos/atomos.rst"
    # "${SPHINX_ROOT_DIRECTORY}/pulse/pulse.rst"
    "${SPHINX_ROOT_DIRECTORY}/pulse/cactuar/cactuar.rst"
    "${SPHINX_ROOT_DIRECTORY}/pulse/dahaka/dahaka.rst"
    )

  add_custom_command(OUTPUT ${SPHINX_HTML_INDEX_FILE}
    COMMAND
    ${SPHINX_EXECUTABLE} 
    -b html
    -j auto
    "-Dbreathe_projects.${PROJECT_NAME}=${SPHINX_DOXYGEN_XML_DIR}"
    "-Dbreathe_default_project=${PROJECT_NAME}"
    ${SPHINX_ROOT_DIRECTORY} ${SPHINX_OUTPUT_DIRECTORY}
    # Note incremental build doesn't copy over css/js however I
    # hate having to specify manually so just remember to copy over
    # your css and js if theyre different. I'll add a copy if different later.
    # ${SPHINX_RESTRUCTRED_TEXT} ${SPHINX_CSS} ${SPHINX_JS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    MAIN_DEPENDENCY ${SPHINX_CONF_OUT}
    DEPENDS ${SPHINX_RESTRUCTRED_TEXT} ${DOXYGEN_XML_INDEX_FILE}
    ${SPHINX_CSS} ${SPHINX_JS}
    COMMENT "Generating documentation with Sphinx")

endmacro()
