#
# Library variables
#
file(GLOB_RECURSE REF_SOFT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h

    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

list(APPEND REF_SOFT_SRCS
    "${CMAKE_SOURCE_DIR}/platforms/sdl/id_shared_sdl.cpp"
)

list(APPEND REF_SOFT_DEFS
    ${ZEALOT_GDEFS}
)

list(APPEND REF_SOFT_LIBS
    Zealot_Core
    SDL2
)

list(APPEND REF_SOFT_INC_DIRS
    SDL2
)

#
# Platform specifics
#
if (WIN32)
    list(APPEND REF_SOFT_LIBS
        "winmm"
    )
endif()

#
# Library definition
#
add_library(Zealot_RefSoft SHARED ${REF_SOFT_SRCS})

target_link_libraries(Zealot_RefSoft PRIVATE ${REF_SOFT_LIBS})
target_compile_definitions(Zealot_RefSoft PRIVATE ${REF_SOFT_DEFS})
target_include_directories(Zealot_RefSoft PRIVATE ${REF_SOFT_INC_DIRS})

set_target_properties(Zealot_RefSoft PROPERTIES OUTPUT_NAME "ref_soft")