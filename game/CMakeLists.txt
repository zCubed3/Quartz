#
# Executable variables
#
file(GLOB_RECURSE GAME_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

#
# Executable definition
#
add_library(Zealot_Game SHARED ${GAME_SRCS})

set_target_properties(Zealot_Game PROPERTIES OUTPUT_NAME "${ZEALOT_GAME_NAME}")