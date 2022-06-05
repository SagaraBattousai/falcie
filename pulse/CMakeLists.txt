
set(PULSE_MODULE_ROOT "${MODULE_ROOT}/pulse")
# set(INTERNAL_INCLUDE_ROOT "${PULSE_INCLUDE_ROOT}/internal")

add_subdirectory(anima)

#add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(dahaka)

set(PULSE_MODULE_ROOT "${MODULE_ROOT}/pulse")

set(pulse_module "${PULSE_MODULE_ROOT}/pulse.ixx")

#Currently CMake Can't handle making a dll with C++20 Modules and 
#setting CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS; therfore, for now, we 
#shall make it static.
add_library(pulse STATIC #SHARED 
  ${pulse_module})

#Required to fix issue with MSVC Expecting a corresponding
#.cpp and therefore looks for both *.ixx.obj and *.obj
add_pseudo_module_impl_objects(pulse ${pulse_module})


target_link_libraries(pulse PUBLIC anima cactuar dahaka)


set_target_properties(pulse anima cactuar dahaka PROPERTIES FOLDER Pulse)

if(MSVC)
  # Add module dir to help intelisense
  # Irritatingly its global but works well and I think it's better than
  # target_sources ?!!?
  set_target_properties(pulse PROPERTIES 
    VS_GLOBAL_PublicModuleDirectories 
    "${PULSE_MODULE_ROOT};$(PublicModuleDirectories)")
endif()





