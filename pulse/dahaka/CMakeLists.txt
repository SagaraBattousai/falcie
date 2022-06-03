###################################################################
################ Dahaka The Container Target ######################
###################################################################

set(DAHAKA_MODULE_ROOT "${PULSE_MODULE_ROOT}/dahaka")

set(dahaka_module 
  "${DAHAKA_MODULE_ROOT}/dahaka.ixx"
  "${DAHAKA_MODULE_ROOT}/dahaka-chain.ixx" 
  "${DAHAKA_MODULE_ROOT}/dahaka-blockchain.ixx" 
  "${DAHAKA_MODULE_ROOT}/dahaka-orderedlist.ixx" 
  "${DAHAKA_MODULE_ROOT}/dahaka-tree.ixx"
)


#Can't be Object anymore as add_custom_command for bost build wont work.
#Static as we want pulse to be the dll and we can statically link the dll to 
#this library and then only link to dll from client.
add_library(dahaka STATIC ${dahaka_module})
  
if(MSVC)
  #Required to fix issue with MSVC Expecting a corresponding
  #.cpp and therefore looks for both *.ixx.obj and *.obj
  add_pseudo_module_impl_objects(dahaka ${dahaka_module})
  #^^ Could've used SOURCES property/gen_expr but meh, we'll see.
endif()

