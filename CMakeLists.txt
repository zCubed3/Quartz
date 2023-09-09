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
option(ZEALOT_BUILD_REF_SOFT "Builds the RefSoft (Software Renderer) module" ON)

# TODO: BuildInfo change platform
include(BuildInfo.txt)

#
# Dependencies
#
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)

#
# Platforms
#
include(qcommon/CMakeLists.txt)

add_subdirectory(platforms/sdl)
set(ZEALOT_Q2_PLAT Zealot_PlatSDL)

#
# Quake modules
#
if (ZEALOT_BUILD_REF_SOFT)
    add_subdirectory(ref_soft)
endif()

if (ZEALOT_BUILD_REF_GL)
    add_subdirectory(third_party/glad_gl2)
    add_subdirectory(ref_gl)
endif()

#
# Quake client and game
#
add_subdirectory(game)
add_subdirectory(client)