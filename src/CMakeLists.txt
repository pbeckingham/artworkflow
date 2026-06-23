cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/commands
                     ${CMAKE_SOURCE_DIR}/src/libshared
                     ${ARTWORKFLOW_INCLUDE_DIRS}
                     "/opt/homebrew/include/lua")
link_directories ("/opt/homebrew/lib")

set (artworkflow_SRCS AtomicFile.cpp     AtomicFile.h
                      CLI.cpp            CLI.h
                      Config.cpp         Config.h
                      Database.cpp       Database.h
                      Datafile.cpp       Datafile.h
                      Exhibition.cpp     Exhibition.h
                      Painting.cpp       Painting.h
                      debug.cpp
                      dom.cpp
                      init.cpp
                      paths.cpp          paths.h
                      util.cpp)

add_library (artworkflow STATIC ${artworkflow_SRCS})
add_executable (artworkflow_executable artworkflow.cpp)

target_link_libraries (artworkflow_executable artworkflow commands shared lua ${ARTWORKFLOW_LIBRARIES})
set_property (TARGET artworkflow_executable PROPERTY OUTPUT_NAME "artworkflow")
install (TARGETS artworkflow_executable DESTINATION ${ARTWORKFLOW_BINDIR})
