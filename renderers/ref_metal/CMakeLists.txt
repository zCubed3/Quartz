#
# Library variables
#
file(GLOB_RECURSE REF_MTL_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h

    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

list(APPEND REF_MTL_SRCS
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_shared_sdl.cpp"

    "${CMAKE_SOURCE_DIR}/platforms/sdl/ref_gl4/gl4_sdl.cpp"
)

list(APPEND REF_MTL_DEFS
    ${ZEALOT_GDEFS}
)

list(APPEND REF_MTL_LIBS
    Zealot_Core
    SDL2
)

list(APPEND REF_MTL_INC_DIRS
    SDL2
)

#
# Optional features
#
if (ZEALOT_USE_IMGUI)
    list(APPEND REF_MTL_SRCS
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_demo.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl2.cpp"
        "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_metal.mm"
    )

    list(APPEND REF_MTL_INC_DIRS
        "${CMAKE_SOURCE_DIR}/third_party/imgui"
    )
endif()

#
# Platform specifics
#
if (NOT APPLE)
    message("Only Apple platforms can use the metal renderer!")
endif()

#
# Library definition
#
add_library(Zealot_RefMTL SHARED ${REF_MTL_SRCS})

target_link_libraries(Zealot_RefMTL PRIVATE ${REF_MTL_LIBS})
target_compile_definitions(Zealot_RefMTL PRIVATE ${REF_MTL_DEFS})
target_include_directories(Zealot_RefMTL PRIVATE ${REF_MTL_INC_DIRS})

set_target_properties(Zealot_RefMTL PROPERTIES OUTPUT_NAME "ref_mtl")