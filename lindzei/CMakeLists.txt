
set(LINDZEI_MODULE_ROOT "${MODULE_ROOT}/lindzei")

add_subdirectory(orphan)

#add_subdirectory(eden)
#add_subdirectory(kujata)

add_subdirectory(cocoon)

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(lindzei_module "${LINDZEI_MODULE_ROOT}/lindzei.h") #.ixx")

add_library(lindzei SHARED ${lindzei_module} $<TARGET_OBJECTS:orphan>)

target_link_libraries(lindzei PUBLIC orphan) # eden kujata)


set_target_properties(lindzei orphan cocoon PROPERTIES FOLDER Lindzei)

target_sources(lindzei PUBLIC
  FILE_SET headers 
  TYPE HEADERS
  BASE_DIRS ${MODULE_ROOT}
  FILES ${lindzei_module}
  )

# if(MSVC)
#   intelisense_modules(lindzei ${LINDZEI_MODULE_ROOT})
# endif()








