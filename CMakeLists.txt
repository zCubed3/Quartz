#
# CMake Project Setup
#
cmake_minimum_required(VERSION 3.15)

project(ZealotQuake2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_CXX_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}/")
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

#
# Global definitions
#
list(APPEND ZEALOT_GDEFS
    _CRT_SECURE_NO_WARNINGS
    C_ONLY
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    USE_IMGUI
)

#
# Project options
#
option(ZEALOT_BUILD_REF_GL2 "Builds ref_gl2 (OpenGL 2.0 Renderer)" ON)
option(ZEALOT_BUILD_REF_GL4 "Builds ref_gl4 (OpenGL 4.6 Renderer)" ON)

option(ZEALOT_USE_IMGUI "Includes Dear ImGui support into the engine and modules" ON)

include(BuildConfig.txt)
# TODO: Generate a BuildConfig.gen.h file

#
# Optional modules
#
if (ZEALOT_USE_IMGUI)
    list(APPEND ZEALOT_GDEFS "USE_IMGUI")
endif()

#
# Dependencies
#
add_subdirectory(third_party/SDL)

#
# Quake modules
#
include(qcommon/CMakeLists.txt)

add_subdirectory(qcore)

if (ZEALOT_BUILD_REF_GL2)
    add_subdirectory(third_party/glad_gl2)
    add_subdirectory(renderers/ref_gl2)
endif()

if (ZEALOT_BUILD_REF_GL4)
    add_subdirectory(third_party/glad_gl4)
    add_subdirectory(renderers/ref_gl4)
endif()

# TODO: Make this prototype library an actual part of the engine
add_subdirectory(zealcore)

#
# Quake client and game
#
add_subdirectory(${ZEALOT_GAME_DIR})
add_subdirectory(client)