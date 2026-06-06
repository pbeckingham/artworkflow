cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/commands
                     ${CMAKE_SOURCE_DIR}/src/libshared/src)

set (commands_SRCS CmdConfig.cpp
                   CmdDefault.cpp
                   CmdDiagnostics.cpp
                   CmdGet.cpp
                   CmdHelp.cpp
		   CmdShow.cpp
                   CmdVersion.cpp)
add_library (commands STATIC ${commands_SRCS})
