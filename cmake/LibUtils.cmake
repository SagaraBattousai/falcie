
#Using advice from cmake generator expressions just always copy is fine
#instead of an awesome script
function(copy_required_dlls target)
  add_custom_command(TARGET ${target} 
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy -t 
      $<TARGET_FILE_DIR:${target}>
      $<TARGET_RUNTIME_DLLS:${target}>
    COMMAND_EXPAND_LISTS
)
endfunction()

