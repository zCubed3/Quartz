add_library(GLAD_GL2 INTERFACE "src/glad.c")

target_include_directories(GLAD_GL2 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")