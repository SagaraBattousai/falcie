
add_subdirectory(anima)

#add_subdirectory(atomos)

add_subdirectory(cactuar)

add_subdirectory(dahaka)

# add_library(pulse INTERFACE)
add_library(pulse SHARED
  $<TARGET_OBJECTS:anima> 
  $<TARGET_OBJECTS:cactuar>
  $<TARGET_OBJECTS:dahaka>)

target_link_libraries(pulse PRIVATE
  anima  
  cactuar
  dahaka)




