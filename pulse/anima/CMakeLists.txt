###################################################################
################## Anima The AI Target ############################
###################################################################

set(ANIMA_MODULE_ROOT "${PULSE_MODULE_ROOT}/anima")

set(anima_module 
  "${ANIMA_MODULE_ROOT}/anima.ixx"
  "${ANIMA_MODULE_ROOT}/anima-matrix.ixx" 
  "${ANIMA_MODULE_ROOT}/anima-neural_network.ixx"
  "${ANIMA_MODULE_ROOT}/anima-random.ixx" 
  )

set(cpp_files 
  "anima-matrix.cpp" 
  "anima-neural_network.cpp"
  "anima-random.cpp")


#Can't be Object anymore as add_custom_command for bost build wont work.
#Static as we want pulse to be the dll and we can statically link the dll to 
#this library and then only link to dll from client.
add_library(anima STATIC ${anima_module} ${cpp_files})

if(MSVC)
  intelisense_modules(anima ${ANIMA_MODULE_ROOT})
endif()

if(MSVC)
  # Add module dir to help intelisense
  # Irritatingly its global but works well and I think it's better than
  # target_sources ?!!?
  set_target_properties(anima PROPERTIES 
    VS_GLOBAL_PublicModuleDirectories 
    "${ANIMA_MODULE_ROOT};$(PublicModuleDirectories)")
endif()


if(x86)
  #Add compile option for AVX Intrinsics
  if(MSVC)
    target_compile_options(anima PRIVATE "/arch:AVX512")
  else()
    #Risky as I've only check it's accaptable for GCC and Clang.
    # Do I need the specific -mavx512 extension versions?
    target_compile_options(anima PRIVATE "-mavx2")
  endif()

  # I can't trust that __AVX__ is defined. (Or can I?).
  # Leave PUBLIC in case client program wants to test it
  target_compile_definitions(anima PUBLIC USE_AVX_INTRIN)
endif()

