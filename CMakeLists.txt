#
# CMake Project Setup
#
cmake_minimum_required(VERSION 3.15)

project(ZealotQuake2)

set(CMAKE_C_STANDARD 99)

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
)

#
# Project options
#
option(ZEALOT_BUILD_REF_GL "Builds the RefGL (OpenGL Renderer) module" ON)

include(BuildConfig.txt)
# TODO: Generate a BuildConfig.gen.h file

#
# Dependencies
#
add_subdirectory(third_party/SDL)

#
# Quake modules
#
include(qcommon/CMakeLists.txt)

if (ZEALOT_BUILD_REF_GL)
    add_subdirectory(third_party/glad_gl2)
    add_subdirectory(ref_gl)
endif()

#
# Quake client and game
#
add_subdirectory(${ZEALOT_GAME_DIR})
add_subdirectory(client)