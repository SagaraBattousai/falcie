
set(LINDZEI_MODULE_ROOT "${MODULE_ROOT}/lindzei")

add_subdirectory(orphan)

#add_subdirectory(eden)
#add_subdirectory(kujata)

add_subdirectory(cocoon)

set(lindzei_module "${LINDZEI_MODULE_ROOT}/lindzei.ixx")

#Currently CMake Can't handle making a dll with C++20 Modules and 
#setting CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS; therfore, for now, we 
#shall make it static.
add_library(lindzei STATIC ${lindzei_module})

#Required to fix issue with MSVC Expecting a corresponding
#.cpp and therefore looks for both *.ixx.obj and *.obj
add_pseudo_module_impl_objects(lindzei ${lindzei_module})

#pulse???
target_link_libraries(lindzei PUBLIC orphan) # eden kujata)


set_target_properties(lindzei orphan cocoon PROPERTIES FOLDER Lindzei)










