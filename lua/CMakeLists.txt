cmake_minimum_required (VERSION 3.10)
#set (CMAKE_CXX_STANDARD 2c)
#set (CMAKE_CXX_STANDARD 23)
#set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set (HAVE_CMAKE true)

project (ltest)

include_directories (${CMAKE_SOURCE_DIR} "/opt/homebrew/include/lua")
link_directories ("/opt/homebrew/lib")
add_executable (ltest_executable main.cpp)
target_link_libraries (ltest_executable lua)
set_property (TARGET ltest_executable PROPERTY OUTPUT_NAME "ltest")

