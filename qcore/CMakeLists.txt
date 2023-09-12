#
# Library variables
#
file(GLOB_RECURSE CORE_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

#
# Library definition
#
add_library(Zealot_Core STATIC ${CORE_SRCS})