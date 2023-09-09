#
# CMake Project Setup
#
cmake_minimum_required(VERSION 3.15)

project(ZealotQuake2)

set(CMAKE_C_STANDARD 99)

#
# Global definitions
#
list(APPEND ZEALOT_GDEFS
    _CRT_SECURE_NO_WARNINGS
    C_ONLY
)

#
# Project options
#
option(ZEALOT_BUILD_REF_GL "Builds the RefGL (OpenGL Renderer) module" ON)

#
# Project modules
#
include(qcommon/CMakeLists.txt)

if (ZEALOT_BUILD_REF_GL)
    add_subdirectory(ref_gl)
endif()