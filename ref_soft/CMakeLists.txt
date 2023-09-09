#
# Library variables
#
file(GLOB_RECURSE REF_SOFT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

list(APPEND REF_SOFT_SRCS
    "${CMAKE_SOURCE_DIR}/game/q_shared.c"
)

list(APPEND REF_SOFT_LIBS
    ${ZEALOT_Q2_PLAT}
)

#
# Library definition
#
add_library(Zealot_RefSoft SHARED ${REF_SOFT_SRCS})

target_link_libraries(Zealot_RefSoft PRIVATE ${REF_SOFT_LIBS})

set_target_properties(Zealot_RefSoft PROPERTIES OUTPUT_NAME "ref_soft")