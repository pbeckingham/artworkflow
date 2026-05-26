cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
		     ${ARTWORKFLOW_INCLUDE_DIRS})

set (artworkflow_SRCS artworkflow.cpp   artworkflow.h
                      Args.cpp          Args.h
                      artworkflow.cpp   artworkflow.h
                      Color.cpp         Color.h
                      Composite.cpp     Composite.h
                      Configuration.cpp Configuration.h
                      Datetime.cpp      Datetime.h
		      debug.cpp
                      Duration.cpp      Duration.h
                      format.cpp        format.h
                      FS.cpp            FS.h
                      ip.cpp
                      JSON.cpp          JSON.h
                      lex.cpp
                      Lexer.cpp         Lexer.h
                      Log.cpp           Log.h
                      Msg.cpp           Msg.h
                      Packrat.cpp       Packrat.h
                      Palette.cpp       Palette.h
                      PEG.cpp           PEG.h
                      Pig.cpp           Pig.h
                      RX.cpp            RX.h
                      SAX.cpp
                      shared.cpp        shared.h
                      Table.cpp         Table.h
                      Timer.cpp         Timer.h
                      Tree.cpp          Tree.h
                      unicode.cpp       unicode.h
                      utf8.cpp          utf8.h
                                        wcwidth.h)

add_executable (artworkflow_executable artworkflow.cpp debug.cpp format.cpp shared.cpp utf8.cpp Color.cpp Timer.cpp)
target_link_libraries (artworkflow_executable)
set_property (TARGET artworkflow_executable PROPERTY OUTPUT_NAME "artworkflow")
install (TARGETS artworkflow_executable DESTINATION ${ARTWORKFLOW_BINDIR})
