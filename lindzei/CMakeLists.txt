
set(LINDZEI_INCLUDE_ROOT "${INCLUDE_ROOT}/lindzei")
# set(INTERNAL_INCLUDE_ROOT "${LINDZEI_INCLUDE_ROOT}/internal")

add_subdirectory(orphan)

#add_subdirectory(eden)
#add_subdirectory(kujata)

add_subdirectory(cocoon)

set_target_properties(orphan cocoon PROPERTIES FOLDER Lindzei)






