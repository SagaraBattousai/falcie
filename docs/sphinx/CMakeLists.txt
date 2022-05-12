
macro(configure_sphinx_and_add_command)

  #Order matters
  set(SPHINX_CSS_FILES_LIST "
  \"css/style.css\",
  \"css/colour.css\",
  \"css/defaults.css\",
  \"css/dark.css\",
  \"css/light.css\",
  ")

  set(SPHINX_JS_FILES_LIST "") #"\"js/falcie_docs.js\","

  set(BREATHE_DOMAIN_BY_EXTENSION_DICT "\"h\" : \"c\",")

  #dont need * and path but A I like path and B ...
  #I cant seem to use ../../ soooo......
  set(BREATHE_DOMAIN_BY_FILE_PATTERN_DICT "
  \"*/include/pulse/chain.h\"       : \"cpp\",
  \"*/include/pulse/blockchain.h\"  : \"cpp\",
  \"*/include/pulse/orderedlist.h\" : \"cpp\",")

  configure_file(${SPHINX_CONF_IN} ${SPHINX_CONF_OUT} @ONLY)

  # As this is a macro, this will be the directory of the caller! :)
  set(SPHINX_DOXYGEN_XML_DIR 
    "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUTPUT_DIRECTORY}/xml")

  #I don't like the glob here but since there are so many files and this
  #file is already messy. Just be sure to run cmake .. if you add an new
  #.rst file
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
