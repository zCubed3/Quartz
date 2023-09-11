#
# Executable variables
#
file(GLOB_RECURSE CLIENT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

include(${CMAKE_SOURCE_DIR}/server/CMakeLists.txt)

list(APPEND CLIENT_SRCS
    ${SERVER_SRCS}

    "${QCOMMON_DIR}/crc.c"
    "${QCOMMON_DIR}/cmd.c"
    "${QCOMMON_DIR}/cmodel.c"
    "${QCOMMON_DIR}/common.c"
    "${QCOMMON_DIR}/cvar.c"
    "${QCOMMON_DIR}/files.c"
    "${QCOMMON_DIR}/net_chan.c"
    "${QCOMMON_DIR}/pmove.c"
    "${QCOMMON_DIR}/md4.c"
    "${QCOMMON_DIR}/qlib.c"

    "${CMAKE_SOURCE_DIR}/game/q_shared.c"
    "${CMAKE_SOURCE_DIR}/game/m_flash.c"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/in_sdl.c"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/sys_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/vid_sdl.c"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/q_shsdl.c"

    "${CMAKE_SOURCE_DIR}/platforms/null/cd_null.c" # TODO: Replace CD Audio!
)

list(APPEND CLIENT_LIBS
    SDL2
    SDL2main
)

list(APPEND CLIENT_INC_DIRS
    SDL2
)

list(APPEND CLIENT_DEFS
    ${ZEALOT_GDEFS}
)

#
# Platform specifics
#
if (WIN32)
    list(APPEND CLIENT_SRCS
        "${CMAKE_SOURCE_DIR}/platforms/sdl/net_wins.c"
        "${CMAKE_SOURCE_DIR}/platforms/sdl/snd_win.c"
        "${CMAKE_SOURCE_DIR}/platforms/sdl/vid_menu.c"

        "${CMAKE_SOURCE_DIR}/platforms/sdl/q2.rc"
    )

    list(APPEND CLIENT_LIBS
        "winmm"
        "ws2_32"
    )
endif()

#
# Executable definition
#
add_executable(Zealot_Client ${CLIENT_SRCS})

target_link_libraries(Zealot_Client PRIVATE ${CLIENT_LIBS})
target_include_directories(Zealot_Client PRIVATE ${CLIENT_INC_DIRS})
target_compile_definitions(Zealot_Client PRIVATE ${CLIENT_DEFS})

set_target_properties(Zealot_Client PROPERTIES OUTPUT_NAME "${ZEALOT_CLIENT_NAME}")

add_dependencies(Zealot_Client Zealot_RefGL Zealot_Game)