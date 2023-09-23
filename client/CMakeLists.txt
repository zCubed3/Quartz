#
# Executable variables
#
file(GLOB_RECURSE CLIENT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

include(${CMAKE_SOURCE_DIR}/server/CMakeLists.txt)

list(APPEND CLIENT_SRCS
    ${SERVER_SRCS}

    "${QCOMMON_DIR}/crc.c"
    "${QCOMMON_DIR}/cmodel.c"
    "${QCOMMON_DIR}/common.cpp"
    "${QCOMMON_DIR}/files.c"
    "${QCOMMON_DIR}/net_chan.c"
    "${QCOMMON_DIR}/pmove.c"
    "${QCOMMON_DIR}/md4.c"

    "${CMAKE_SOURCE_DIR}/games/quake2/q_shared.c"
    "${CMAKE_SOURCE_DIR}/games/quake2/m_flash.c"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/vid_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/vid_menu.c"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_input_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_system_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_shared_sdl.cpp"

    "${CMAKE_SOURCE_DIR}/platforms/null/cd_null.c" # TODO: Replace CD Audio!

    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_demo.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl2.cpp"
)

list(APPEND CLIENT_LIBS
    ZealCore
    Zealot_Core
    SDL2
    SDL2main
)

list(APPEND CLIENT_INC_DIRS
    ZealCore
    SDL2
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

list(APPEND CLIENT_DEFS
    ${ZEALOT_GDEFS}
)

#
# Platform specifics
#
if (WIN32)
    list(APPEND CLIENT_SRCS
        "${CMAKE_SOURCE_DIR}/platforms/sdl/win32/net_wins.c"
        "${CMAKE_SOURCE_DIR}/platforms/sdl/win32/snd_win.c"

        "${CMAKE_SOURCE_DIR}/platforms/sdl/q2.rc"
    )

    list(APPEND CLIENT_LIBS
        "winmm"
        "ws2_32"
    )
endif()

if (APPLE)
    list(APPEND CLIENT_SRCS
        "${CMAKE_SOURCE_DIR}/platforms/sdl/posix/net_posix.c"
        "${CMAKE_SOURCE_DIR}/platforms/null/snddma_null.c"
    )
endif()

#
# Static linking
#
if (ZEALOT_STATIC_LINK)
    list(APPEND CLIENT_DEFS
        ZEALOT_STATIC_LINK
    )

    list(APPEND CLIENT_LIBS
        ${ZEALOT_STATIC_REF}
        ${ZEALOT_STATIC_GAME}
    )
endif()

#
# Bundled resources
#
if (APPLE)
    # TODO REMOVE THIS JANK PROTOTYPE CODE!
    file(GLOB_RECURSE CLIENT_RES_FILES
        "${CMAKE_SOURCE_DIR}/out/baseq2/*"
    )

    message("FILES = ${CLIENT_RES_FILES}")

    foreach(RES_FILE ${CLIENT_RES_FILES})
        file(RELATIVE_PATH RES_PATH "${CMAKE_SOURCE_DIR}/out/baseq2" ${RES_FILE})

        get_filename_component(RES_FILE_PATH ${RES_PATH} DIRECTORY)

        #Set it's location inside the app package (under Resources)
        set_property(SOURCE ${RES_FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/baseq2/${RES_FILE_PATH}")

        message("RES_FILE = ${RES_FILE_PATH}")

        list(APPEND CLIENT_SRCS ${RES_FILE})
    endforeach ()
endif()

#
# Executable definition
#
add_executable(Zealot_Client ${CLIENT_SRCS})

target_link_libraries(Zealot_Client PRIVATE ${CLIENT_LIBS})
target_include_directories(Zealot_Client PRIVATE ${CLIENT_INC_DIRS})
target_compile_definitions(Zealot_Client PRIVATE ${CLIENT_DEFS})

set_target_properties(Zealot_Client PROPERTIES OUTPUT_NAME "${ZEALOT_CLIENT_NAME}")

#
# Apple platform info
#
if (APPLE)
    set_target_properties(Zealot_Client PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "Zealot.Client" # TODO Get this from the private build info
        MACOSX_BUNDLE_SHORT_VERSION_STRING 0.1.0
        MACOSX_BUNDLE_BUNDLE_VERSION 0.1.0
    )
endif()