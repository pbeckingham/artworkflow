cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/commands
                     ${CMAKE_SOURCE_DIR}/src/libshared
		     ${ARTWORKFLOW_INCLUDE_DIRS})

set (artworkflow_SRCS AtomicFile.cpp     AtomicFile.h
                      CLI.cpp            CLI.h
                      DatetimeParser.cpp DatetimeParser.h
                      Database.cpp       Database.h
                      Datafile.cpp       Datafile.h
                      Rules.cpp          Rules.h
                      debug.cpp
                      init.cpp
                      util.cpp)

add_library (artworkflow STATIC ${artworkflow_SRCS})
add_executable (artworkflow_executable artworkflow.cpp)

target_link_libraries (artworkflow_executable artworkflow commands shared ${ARTWORKFLOW_LIBRARIES})
set_property (TARGET artworkflow_executable PROPERTY OUTPUT_NAME "artworkflow")
install (TARGETS artworkflow_executable DESTINATION ${ARTWORKFLOW_BINDIR})
