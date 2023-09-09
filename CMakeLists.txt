#
# CMake Project Setup
#
cmake_minimum_required(VERSION 3.15)

project(ZealotQuake2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_C_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}/")
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

#
# Global variables
#
list(APPEND ZEALOT_GDEFS
    _CRT_SECURE_NO_WARNINGS
    C_ONLY
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

#
# Project options
#
option(ZEALOT_BUILD_REF_GL "Builds the RefGL (OpenGL Renderer) module" ON)

# TODO: Include this from a BuildInfo.txt file
set(ZEALOT_CLIENT_NAME "quake2")

# TODO: Change quake to not depend on post-fixed DLLs
set(ZEALOT_GAME_NAME "gamex64")

#
# Dependencies
#
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

#
# Quake modules
#
include(qcommon/CMakeLists.txt)

if (ZEALOT_BUILD_REF_GL)
    add_subdirectory(ref_gl)
endif()

#
# Quake client and game
#
add_subdirectory(game)
add_subdirectory(client)