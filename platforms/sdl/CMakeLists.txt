#
# Library variables
#
file(GLOB PLAT_SDL_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

list(APPEND PLAT_SDL_LIBS
    SDL2
    SDL2main
)

list(APPEND PLAT_SDL_INC_DIRS
    SDL2
)

#
# Platform dependencies
#
if (WIN32)
    list(APPEND PLAT_SDL_SRCS
        ${CMAKE_CURRENT_LIST_DIR}/win/net_sdl_win.c
        ${CMAKE_CURRENT_LIST_DIR}/win/q_shsdl_win.c
    )

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


#
# Ref module dependencies
#
if (ZEALOT_BUILD_REF_GL)
    list(APPEND PLAT_SDL_REF_GL_SRCS
        ${CMAKE_CURRENT_LIST_DIR}/gl/qgl_sdl.c
        ${CMAKE_CURRENT_LIST_DIR}/gl/glimp_sdl.c
        ${CMAKE_CURRENT_LIST_DIR}/q_shsdl.c
    )

    list(APPEND PLAT_SDL_REF_GL_LIBS
        SDL2
        GLAD_GL2
    )

    list(APPEND PLAT_SDL_REF_GL_INC_DIRS
        SDL2
        GLAD_GL2
    )

    if (WIN32)
        list(APPEND PLAT_SDL_REF_GL_SRCS
            ${CMAKE_CURRENT_LIST_DIR}/win/q_shsdl_win.c
        )
    endif()


    add_library(Zealot_PlatSDL_RefGL STATIC ${PLAT_SDL_REF_GL_SRCS})

    target_include_directories(Zealot_PlatSDL_RefGL PUBLIC ${PLAT_SDL_REF_GL_INC_DIRS})

    target_link_libraries(Zealot_PlatSDL_RefGL PUBLIC ${PLAT_SDL_REF_GL_LIBS})
endif()