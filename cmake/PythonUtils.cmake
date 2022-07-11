# Could make type {SHARED, STATIC} an arg but for now should be SHARED
function(add_python_extension target sources)
  add_library(${target} SHARED ${sources})

  if (WIN32)
    set(python_library_extension ".pyd")
  else()
    set(python_library_extension ".so")
  endif()

  add_custom_command(TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    $<TARGET_FILE:${target}>
    $<TARGET_FILE_BASE_NAME:${target}>${python_library_extension}
    COMMENT "Creating python extension called: ${target} \
with extension: ${python_library_extension}.")

endfunction()

