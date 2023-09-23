#
# Executable variables
#
file(GLOB_RECURSE GAME_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

#
# Library types
#
if (ZEALOT_STATIC_LINK)
    set(GAME_LIB_TYPE STATIC)
    set(GAME_DEFS "GAME_HARD_LINKED")
else()
    set(GAME_LIB_TYPE SHARED)
endif()

#
# Executable definition
#
add_library(Zealot_Game ${GAME_LIB_TYPE} ${GAME_SRCS})

target_link_libraries(Zealot_Game PRIVATE Zealot_Core)

target_compile_definitions(Zealot_Game PRIVATE ${GAME_DEFS})

set_target_properties(Zealot_Game PROPERTIES OUTPUT_NAME "${ZEALOT_GAME_NAME}")