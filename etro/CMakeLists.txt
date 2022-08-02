
set(ETRO_MODULE_ROOT "${MODULE_ROOT}/etro")

add_subdirectory(python)

set(sources "etro.c")

add_library(etro SHARED ${sources})

# target_sources(etro PUBLIC
#   FILE_SET headers
#   TYPE HEADERS
#   BASE_DIRS ${MODULE_ROOT}
#   FILES
#   )


set_target_properties(etro falpy PROPERTIES FOLDER Etro)
