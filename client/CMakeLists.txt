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

    "${CMAKE_SOURCE_DIR}/game/q_shared.c"
    "${CMAKE_SOURCE_DIR}/game/m_flash.c"

    "${CMAKE_SOURCE_DIR}/null/cd_null.c" # TODO: Replace CD Audio!
)

#
# Platform specifics
#
if (WIN32)
    list(APPEND CLIENT_SRCS
        "${CMAKE_SOURCE_DIR}/win32/in_win.c"
        "${CMAKE_SOURCE_DIR}/win32/net_wins.c"
        "${CMAKE_SOURCE_DIR}/win32/snd_win.c"
        "${CMAKE_SOURCE_DIR}/win32/sys_win.c"
        "${CMAKE_SOURCE_DIR}/win32/vid_menu.c"
        "${CMAKE_SOURCE_DIR}/win32/vid_dll.c"
        "${CMAKE_SOURCE_DIR}/win32/q_shwin.c"
        "${CMAKE_SOURCE_DIR}/win32/conproc.c"

        "${CMAKE_SOURCE_DIR}/win32/q2.rc"
    )

    list(APPEND CLIENT_LIBS
        "winmm"
        "ws2_32"
    )

    set(BUILD_TYPE WIN32)
endif()

#
# Executable definition
#
add_executable(Zealot_Client ${BUILD_TYPE} ${CLIENT_SRCS})

target_link_libraries(Zealot_Client PRIVATE ${CLIENT_LIBS})

set_target_properties(Zealot_Client PROPERTIES OUTPUT_NAME "${ZEALOT_CLIENT_NAME}")