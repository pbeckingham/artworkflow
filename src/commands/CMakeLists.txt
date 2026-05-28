cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/commands
                     ${CMAKE_SOURCE_DIR}/src/libshared/src)

set (commands_SRCS CmdDiagnostics.cpp
                   CmdHelp.cpp)
add_library (commands STATIC ${commands_SRCS})
