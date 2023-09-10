#
# Library variables
#
file(GLOB_RECURSE REF_GL_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

list(APPEND REF_GL_SRCS
    "${CMAKE_SOURCE_DIR}/game/q_shared.c"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/q_shsdl.c"
)

list(APPEND REF_GL_DEFS
    ${ZEALOT_GDEFS}
)

list(APPEND REF_GL_LIBS
    SDL2
)

list(APPEND REF_GL_INC_DIRS
    SDL2
)


#
# Platform specifics
#
if (WIN32)
    set(OPENGL_LIB "opengl32")

    list(APPEND REF_GL_SRCS
        "${CMAKE_SOURCE_DIR}/platforms/sdl/glw_imp.c"
        "${CMAKE_SOURCE_DIR}/platforms/sdl/qgl_win.c"

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

target_link_libraries(Zealot_RefGL PRIVATE ${REF_GL_LIBS})
target_compile_definitions(Zealot_RefGL PRIVATE ${REF_GL_DEFS})
target_include_directories(Zealot_RefGL PRIVATE ${REF_GL_INC_DIRS})

set_target_properties(Zealot_RefGL PROPERTIES OUTPUT_NAME "ref_gl")