#
# Library Definition
#
add_library(GLAD_GL2 STATIC "src/glad.c")
target_include_directories(GLAD_GL2 PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")