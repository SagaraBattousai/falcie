#Expects list of .ixx files (with paths is okay) to know what to create
#For MSVC adds the object files corresponding to the "expected"
#Module implementation files; these don't exist given the way I like to 
#implement them.
function(add_pseudo_module_impl_objects target)
  if(ARGC LESS 2)
    message(WARNING "Needs additional arguments (namely module \
    implementation files (.ixx)) in order to be of use")
    return()
  endif()

  set(INTERMEDIATE_ROOT "${CMAKE_CURRENT_BINARY_DIR}/${target}.dir/$<CONFIG>")

  list(TRANSFORM ARGN REPLACE ".*/(.*)\.ixx$" "${INTERMEDIATE_ROOT}/\\1.obj")

  add_custom_command(TARGET ${target} PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E touch ${ARGN}
    COMMENT "Adding pseudo object files for MSVC's build system"
    )

endfunction()
