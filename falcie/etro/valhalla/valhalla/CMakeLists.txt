
set(VALHALLA_MODULE_ROOT "${MODULE_ROOT}/valhalla")

set(cpp_sources 
  "model.cpp" 
  "model_impl.cpp" 
  "model_function.cpp"
  "model_function_impl.cpp"
  "federated_model.cpp"
  "status_flag.cpp"
  "tensor.cpp"
  "tensor_impl.cpp"
  "type_flag.cpp"
  ) 

set(c_sources "")


set(valhalla_module
  "${VALHALLA_MODULE_ROOT}/federated_model.h"
  "${VALHALLA_MODULE_ROOT}/model.h"
  "${VALHALLA_MODULE_ROOT}/model_function.h"
  "${VALHALLA_MODULE_ROOT}/status_flag.h"
  "${VALHALLA_MODULE_ROOT}/tensor.h"
  "${VALHALLA_MODULE_ROOT}/type_flag.h"
  )

set(private_valhalla_module
  "${VALHALLA_MODULE_ROOT}/internal/model_impl.h"
  "${VALHALLA_MODULE_ROOT}/internal/model_function_impl.h"
  "${VALHALLA_MODULE_ROOT}/internal/tensor_impl.h"
  )

add_library(valhalla SHARED ${cpp_sources} ${c_sources})

target_sources(valhalla PUBLIC
  FILE_SET HEADERS
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${valhalla_module}
  "${MODULE_ROOT}/_falcie_config.h"
  )

# These must not be included in public headers
target_sources(valhalla PRIVATE
  FILE_SET internal_headers
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${private_valhalla_module}
  )

target_link_libraries(valhalla PRIVATE tensorflowlite_c)
# target_link_libraries(valhalla PUBLIC tensorflowlite_c tensorflowlite_flex)
target_link_libraries(valhalla PUBLIC utility)

target_compile_definitions(valhalla PRIVATE FALCIE_BUILD_DLL)
target_compile_options(valhalla PRIVATE ${FALCIE_COMPILE_OPTIONS})
