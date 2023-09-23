#
# Zealot Quake 2 - Build Configuration
#

#
# Client Info
#
set(ZEALOT_CLIENT_NAME "quake2")
set(ZEALOT_CLIENT_VERSION "3.21")

#
# Game Info
#
set(ZEALOT_GAME_DIR "${CMAKE_SOURCE_DIR}/games/quake2")
set(ZEALOT_GAME_NAME "game")
set(ZEALOT_GAME_VERSION "1.0")

#
# Linking Info
#
set(ZEALOT_STATIC_LINK OFF)

if (APPLE)
    set(ZEALOT_STATIC_LINK ON)

    set(ZEALOT_STATIC_REF Zealot_RefGL2)
    set(ZEALOT_STATIC_GAME Zealot_Game)
endif()