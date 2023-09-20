#
# Library variables
#
file(GLOB_RECURSE REF_GL4_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h

    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

list(APPEND REF_GL4_SRCS
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_shared_sdl.cpp"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/ref_gl4/gl4_sdl.cpp"
)

list(APPEND REF_GL4_DEFS
    ${ZEALOT_GDEFS}
)

list(APPEND REF_GL4_LIBS
    Zealot_Core
    SDL2
    GLAD_GL4
)

list(APPEND REF_GL4_INC_DIRS
    SDL2
    GLAD_GL4
)

#
# Optional features
#
if (ZEALOT_USE_IMGUI)
    list(APPEND REF_GL4_SRCS
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_demo.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl2.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp"
    )

    list(APPEND REF_GL4_INC_DIRS
        "${CMAKE_SOURCE_DIR}/third_party/imgui"
    )
endif()

#
# Platform specifics
#
if (WIN32)
    set(OPENGL_LIB "opengl32")

    list(APPEND REF_GL4_LIBS
        "winmm"
    )
endif()

if (UNIX)
    set(OPENGL_LIB "GL")
endif()

list(APPEND REF_GL4_LIBS
    ${OPENGL_LIB}
)

#
# Library definition
#
add_library(Zealot_RefGL4 SHARED ${REF_GL4_SRCS})

target_link_libraries(Zealot_RefGL4 PRIVATE ${REF_GL4_LIBS})
target_compile_definitions(Zealot_RefGL4 PRIVATE ${REF_GL4_DEFS})
target_include_directories(Zealot_RefGL4 PRIVATE ${REF_GL4_INC_DIRS})

set_target_properties(Zealot_RefGL4 PROPERTIES OUTPUT_NAME "ref_gl4")

#
# Output assets
#
add_custom_target(Zealot_RefGL4_CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/shaders
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ref_gl4/shaders
)

add_dependencies(Zealot_RefGL4 Zealot_RefGL4_CopyShaders)