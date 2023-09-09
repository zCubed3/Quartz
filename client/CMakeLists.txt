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

    "${CMAKE_SOURCE_DIR}/null/cd_null.c" # TODO: Replace CD Audio!
)

#
# Platform specifics
#
list(APPEND CLIENT_LIBS
    ${ZEALOT_Q2_PLAT}
)

#
# Executable definition
#
add_executable(Zealot_Client ${CLIENT_SRCS})

target_link_libraries(Zealot_Client PRIVATE ${CLIENT_LIBS})
target_include_directories(Zealot_Client PRIVATE ${CLIENT_INC_DIRS})

set_target_properties(Zealot_Client PROPERTIES OUTPUT_NAME "${ZEALOT_Q2_CLIENT}")