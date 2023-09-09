#
# Library variables
#
file(GLOB_RECURSE PLAT_SDL_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

list(APPEND PLAT_SDL_LIBS
    SDL2
    SDL2main
    GLAD_GL2
)

list(APPEND PLAT_SDL_INC_DIRS
    SDL2
    GLAD_GL2
)

#
# Platform dependencies
#
if (WIN32)
    list(APPEND PLAT_SDL_LIBS
        "ws2_32"
    )
endif()

#
# Library definition
#
add_library(Zealot_PlatSDL STATIC ${PLAT_SDL_SRCS})

target_include_directories(Zealot_PlatSDL PRIVATE ${PLAT_SDL_INC_DIRS})

target_link_libraries(Zealot_PlatSDL PUBLIC ${PLAT_SDL_LIBS})