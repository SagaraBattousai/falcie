function(as_python_extension target)
  if (WIN32)
    set(python_library_extension ".pyd")
  else()
    set(python_library_extension ".so")
  endif()

  set_target_properties(${target} PROPERTIES 
    SUFFIX ${python_library_extension})

endfunction()

