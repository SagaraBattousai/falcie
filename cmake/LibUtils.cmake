
#Keeping others just for now, will be removed in the future. Will use copy as 
#copy if different was causing an issue ( I think/remember)
function(auto_copy_dlls_for_target target)
  add_custom_command(TARGET ${target} 
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_RUNTIME_DLLS:${target}>
    $<TARGET_FILE_DIR:${target}>
  COMMAND_EXPAND_LISTS
)
endfunction()

#Note, small changes to dll can sometime (as in this case and why I 
#am writing this function) result in no change to the dll
#therefore symlinking is better!
function(symlink_library_to_target target lib)
  add_custom_command(TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    $<TARGET_FILE:${lib}>
    $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_NAME:${lib}>
    COMMENT "Creating a symbolic link to ${lib} from Target ${target}'s \
    directory.")
endfunction()

function(copy_library_to_target target lib)
  add_custom_command(TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:${lib}>
    $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_NAME:${lib}>
    USES_TERMINAL #Guess I dont need :D (Is it better without?)
    COMMENT "Copying ${lib} to Target ${target}'s directory")
endfunction()

function(copy_file_to_target target file_to_copy)
  add_custom_command(TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${file_to_copy}
    $<TARGET_FILE_DIR:${target}>
    COMMENT "Copying ${file_to_copy} to Target ${target}'s directory")
endfunction()

