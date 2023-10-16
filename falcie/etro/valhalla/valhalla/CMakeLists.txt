
set(VALHALLA_MODULE_ROOT "${MODULE_ROOT}/valhalla")

set(cpp_sources 
  "model.cpp" 
  "model_impl.cpp" 
  "model_function.cpp"
  "model_function_impl.cpp"
  "federated_model.cpp"
  ) 

set(c_sources "")


set(valhalla_module
  "${VALHALLA_MODULE_ROOT}/model.h"
  "${VALHALLA_MODULE_ROOT}/federated_model.h"
  "${VALHALLA_MODULE_ROOT}/model_function.h"
  )

set(private_valhalla_module
  "${VALHALLA_MODULE_ROOT}/internal/model_impl.h"
  "${VALHALLA_MODULE_ROOT}/internal/model_function_impl.h"
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
target_compile_definitions(valhalla PRIVATE FALCIE_BUILD_DLL)

