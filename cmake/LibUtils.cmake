
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

function(copy_openssl_runtime_lib_to_target target lib)
  get_openssl_runtime_lib(${lib})
  copy_file_to_target(${target} "${OPENSSL_RUNTIME_LIB_${lib}}")
endfunction()


#At the moment only useful for windows as the "*.dll" GLOB is hardcoded
#Could add check for extra extension command but
#As I dont know the dir structure on unix I cant be sure whats best
function(get_openssl_runtime_lib lib)

  if(NOT OPENSSL_ROOT_DIR)
    #Assumes that bin is a sibling of include dir
    cmake_path(GET OPENSSL_INCLUDE_DIR PARENT_PATH ORD)
    set(OPENSSL_ROOT_DIR ${ORD} PARENT_SCOPE)
    set(OPENSSL_ROOT_DIR ${ORD})
  endif()

  file(GLOB runtime_libs 
    LIST_DIRECTORIES false 
    "${OPENSSL_ROOT_DIR}/bin/lib${lib}*.dll")
  list(LENGTH runtime_libs runtime_libs_length)

  if(runtime_libs_length EQUAL 0)
    message(NOTICE 
      "Could not find the required runtime library using "
      "${OPENSSL_ROOT_DIR}/bin/lib${lib}*.dll as the glob expression") 
  endif()

  if(runtime_libs_length GREATER 1)
    message(WARNING 
      "More than one match found, using the first in the"
      "following list:\n${runtime_libs}")
  endif()

  list(GET runtime_libs 0 runtime_lib)
  set(OPENSSL_RUNTIME_LIB_${lib} ${runtime_lib} PARENT_SCOPE)
endfunction()



