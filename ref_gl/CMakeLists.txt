#
# Library variables
#
file(GLOB_RECURSE REF_GL_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

list(APPEND REF_GL_SRCS
    "${CMAKE_SOURCE_DIR}/game/q_shared.c"
)

#
# Platform specifics
#
if (WIN32)
    set(OPENGL_LIB "opengl32")

    list(APPEND REF_GL_SRCS
        "${CMAKE_SOURCE_DIR}/win32/glw_imp.c"
        "${CMAKE_SOURCE_DIR}/win32/qgl_win.c"
        "${CMAKE_SOURCE_DIR}/win32/q_shwin.c"
    )

    list(APPEND REF_GL_LIBS
        "winmm"
    )
endif()

if (UNIX)
    set(OPENGL_LIB "GL")
endif()

list(APPEND REF_GL_LIBS
    ${OPENGL_LIB}
)

#
# Library definition
#
add_library(Zealot_RefGL SHARED ${REF_GL_SRCS})

target_compile_definitions(Zealot_RefGL PRIVATE ${ZEALOT_GDEFS})

target_link_libraries(Zealot_RefGL PRIVATE ${REF_GL_LIBS})

set_target_properties(Zealot_RefGL PROPERTIES OUTPUT_NAME "ref_gl")