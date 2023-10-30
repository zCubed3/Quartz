#
# Library variables
#
file(GLOB REF_GL_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

list(APPEND REF_GL_SRCS
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_shared_sdl.cpp"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/ref_gl2/gl2_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platforms/sdl/ref_gl2/gl2_sdl_imp.cpp"
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
# Optional features
#
if (ZEALOT_USE_IMGUI)
    list(APPEND REF_GL_SRCS
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_demo.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl2.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl2.cpp"
    )

    list(APPEND REF_GL_INC_DIRS
        "${CMAKE_SOURCE_DIR}/third_party/imgui"
    )
endif()

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

if (APPLE)
    set(OPENGL_LIB "-framework OpenGL")
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