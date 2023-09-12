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

    "${CMAKE_SOURCE_DIR}/platforms/sdl/ref_gl2/gl2_sdl.c"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/ref_gl2/gl2_sdl_imp.c"
)

list(APPEND REF_GL_DEFS
    ${ZEALOT_GDEFS}
)

list(APPEND REF_GL_LIBS
    Zealot_Core
    SDL2
    GLAD_GL2
)

list(APPEND REF_GL_INC_DIRS
    SDL2
    GLAD_GL2
)


#
# Platform specifics
#
if (WIN32)
    set(OPENGL_LIB "opengl32")

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
add_library(Zealot_RefGL2 SHARED ${REF_GL_SRCS})

target_link_libraries(Zealot_RefGL2 PRIVATE ${REF_GL_LIBS})
target_compile_definitions(Zealot_RefGL2 PRIVATE ${REF_GL_DEFS})
target_include_directories(Zealot_RefGL2 PRIVATE ${REF_GL_INC_DIRS})

set_target_properties(Zealot_RefGL2 PROPERTIES OUTPUT_NAME "ref_gl2")