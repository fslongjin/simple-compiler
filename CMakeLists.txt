cmake_minimum_required(VERSION 3.21)
project(simple_compiler)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lex)
add_subdirectory(parser)

aux_source_directory(. DIR_SRCS)
add_executable(simple_compiler ${DIR_SRCS})

# 将词法分析器链接进来
target_link_libraries(simple_compiler lex)