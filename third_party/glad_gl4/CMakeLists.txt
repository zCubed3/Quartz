#
# Library Definition
#
add_library(GLAD_GL4 STATIC "src/glad.c")
target_include_directories(GLAD_GL4 PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")