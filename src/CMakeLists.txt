cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
		     ${ARTWORKFLOW_INCLUDE_DIRS})

set (artworkflow_SRCS artworkflow.cpp artworkflow.h)

add_executable (artworkflow_executable artworkflow.cpp)
target_link_libraries (artworkflow_executable)
set_property (TARGET artworkflow_executable PROPERTY OUTPUT_NAME "artworkflow")
install (TARGETS artworkflow_executable DESTINATION ${ARTWORKFLOW_BINDIR})
