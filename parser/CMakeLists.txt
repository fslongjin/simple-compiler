aux_source_directory(. DIR_PARSER_SRCS)

include_directories(.)
add_library(parser ${DIR_PARSER_SRCS})
