###################################################################
################## Anima The AI Target ############################
###################################################################

# Will still use the term "Module" but untill compilers catchup we shall use
# header.

set(ANIMA_MODULE_ROOT "${PULSE_MODULE_ROOT}/anima")

set(anima_module 
  "${ANIMA_MODULE_ROOT}/anima.h" # ixx"
  "${ANIMA_MODULE_ROOT}/anima-matrix.h" # ixx" 
  "${ANIMA_MODULE_ROOT}/anima-neural_network.h" # ixx"
  "${ANIMA_MODULE_ROOT}/anima-random.h" # ixx" 
  )

set(cpp_files 
  "anima-matrix.cpp" 
  "anima-neural_network.cpp"
  "anima-random.cpp")

add_library(anima OBJECT ${anima_module} ${cpp_files})

target_sources(anima PUBLIC
  FILE_SET headers 
  TYPE HEADERS
  BASE_DIRS ${PULSE_MODULE_ROOT}
  FILES ${anima_module}
  )


# if(MSVC)
#   intelisense_modules(anima ${ANIMA_MODULE_ROOT})
# endif()

if(x86)
  #Add compile option for AVX Intrinsics
  if(MSVC)
    target_compile_options(anima PRIVATE "/arch:AVX512")
  else()
    #Risky as I've only check it's accaptable for GCC and Clang.
    # Do I need the specific -mavx512 extension versions?
    target_compile_options(anima PRIVATE "-mavx512f")
  endif()
  # I can't trust that __AVX__ is defined. (Or can I?).
  # Leave PUBLIC in case client program wants to test it
  target_compile_definitions(anima PUBLIC USE_AVX_INTRIN)
endif()

