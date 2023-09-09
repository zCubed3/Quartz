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

list(APPEND REF_GL_LIBS
    ${ZEALOT_Q2_PLAT}
)

#
# Platform specifics
#
if (WIN32)
    list(APPEND REF_GL_LIBS
        "opengl32"
    )
endif()

if (UNIX)
    list(APPEND REF_GL_LIBS
        "GL"
    )
endif()

#
# Library definition
#
add_library(Zealot_RefGL SHARED ${REF_GL_SRCS})

target_compile_definitions(Zealot_RefGL PRIVATE ${ZEALOT_GDEFS})

target_link_libraries(Zealot_RefGL PRIVATE ${REF_GL_LIBS})

set_target_properties(Zealot_RefGL PROPERTIES OUTPUT_NAME "ref_gl")