find_package(Python3 COMPONENTS Development.Module NumPy) # Could add quiet

if (Python3_FOUND)
  set(FALPY_MODULE_ROOT "${MODULE_ROOT}/etro/python/falpy")
  
  set(sources 
    "falpy.c" 
    "falpy_weights.c" 
    "falpy_block.c" 
    "falpy_blockchain.c" 
    "falpy_networkupdate.c" 
    "tf_model_matrix.c")
  
  set(falpy_module 
    "${FALPY_MODULE_ROOT}/_falpyconfig.h"
    "${FALPY_MODULE_ROOT}/falpy.h" # ixx"
    "${FALPY_MODULE_ROOT}/falpy_weights.h" # ixx"
    "${FALPY_MODULE_ROOT}/falpy_block.h" # ixx"
    "${FALPY_MODULE_ROOT}/falpy_blockchain.h" # ixx"
    "${FALPY_MODULE_ROOT}/falpy_networkupdate.h" # ixx"
    "${FALPY_MODULE_ROOT}/tf_model_matrix.h" # ixx"
    )
  
  add_library(falpy SHARED ${sources})
  
  as_python_extension(falpy)
  
  target_sources(falpy PRIVATE
    FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "${MODULE_ROOT}/etro/python"
    FILES ${falpy_module} 
    )
  
  target_include_directories(falpy PUBLIC
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
    )

  target_compile_options(falpy PRIVATE ${FALCIE_COMPILE_OPTIONS})
  
  target_link_libraries(falpy PUBLIC Python3::Module etro)
  target_link_libraries(falpy PRIVATE Python3::NumPy)
  
  if (MSVC)
    set_property(TARGET falpy PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS OFF)
    target_compile_definitions(falpy PRIVATE FALPY_BUILD_DLL)
    target_compile_definitions(falpy PRIVATE 
      NPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)
  
    copy_required_dlls(falpy)
    # copy_openssl_runtime_lib_to_target(falpy crypto)
  
    # symlink_library_to_target(falpy pulse)
    # symlink_library_to_target(falpy lindzei)
    # symlink_library_to_target(falpy etro)
  endif()

endif()





