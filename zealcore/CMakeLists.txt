#
# Library variables
#

file(GLOB_RECURSE ZEALCORE_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
)

#
# Library definition
#
add_library(ZealCore STATIC ${ZEALCORE_SRCS})

#
# Test executable
# TODO: Make this optional
#
add_executable(ZealCore_tester ${ZEALCORE_SRCS})

target_compile_definitions(ZealCore_tester PUBLIC "USE_ZEALCORE_MAIN")