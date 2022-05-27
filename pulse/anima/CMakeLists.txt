###################################################################
################## Anima The AI Target ############################
###################################################################

set(ANIMA_MODULE_ROOT "${PULSE_MODULE_ROOT}/anima")

set(module_partition 
  "${ANIMA_MODULE_ROOT}/anima-matrix.ixx" 
  "${ANIMA_MODULE_ROOT}/anima-random.ixx" 
  "${ANIMA_MODULE_ROOT}/anima-neural_network.ixx")

set(cpp_files 
  "anima-matrix.cpp" 
  "anima-neural_network.cpp"
  "anima-random.cpp")

set(anima_module "${ANIMA_MODULE_ROOT}/anima.ixx")

#Can't be Object anymore as add_custom_command for bost build wont work.
#Static as we want pulse to be the dll and we can statically link the dll to 
#this library and then only link to dll from client.
add_library(anima STATIC ${anima_module} ${module_partition} ${cpp_files})

if(MSVC)
  # Required to fix issue with MSVC when using header only style 
  # (due to template classes for example). In such cases MSVC 
  # expects a corresponding .cpp and therefore looks for both
  # *.ixx.obj and *.obj
  # In This case however we do have an implementation file for anima-matrix
  # as this stores our avx versions.
  add_pseudo_module_impl_objects(anima ${anima_module})
  #^^ Could've used SOURCES property/gen_expr but meh, we'll see.
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

