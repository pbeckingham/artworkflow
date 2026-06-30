cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/commands
                     ${CMAKE_SOURCE_DIR}/src/libshared
                     "/opt/homebrew/include/lua")

set (commands_SRCS CmdDefault.cpp
                   CmdDiagnostics.cpp
                   CmdGet.cpp
                   CmdHelp.cpp
                   CmdInfo.cpp
                   CmdNew.cpp
                   CmdVersion.cpp)
add_library (commands STATIC ${commands_SRCS})
