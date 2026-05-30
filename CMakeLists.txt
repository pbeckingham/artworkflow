cmake_minimum_required (VERSION 3.10)
#set (CMAKE_CXX_STANDARD 2c)
#set (CMAKE_CXX_STANDARD 23)
#set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set (HAVE_CMAKE true)

project (artworkflow)
include (CXXSniffer)
include (FindAsciidoctor)
include (CheckFunctionExists)

set (PROJECT_VERSION "0.0.4-dev")

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

message ("-- Looking for SHA1 references")
if (EXISTS ${CMAKE_SOURCE_DIR}/.git/index)
  set (HAVE_COMMIT true)
  execute_process (COMMAND git log -1 --pretty=format:%h
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                   OUTPUT_VARIABLE COMMIT)
  configure_file (${CMAKE_SOURCE_DIR}/commit.h.in
                  ${CMAKE_SOURCE_DIR}/commit.h)
  message ("-- Found SHA1 reference: ${COMMIT}")
endif (EXISTS ${CMAKE_SOURCE_DIR}/.git/index)

set (PACKAGE "${PROJECT_NAME}")
set (VERSION "${PROJECT_VERSION}")
set (PACKAGE_BUGREPORT "paul@beckingham.net")
set (PACKAGE_NAME "${PACKAGE}")
set (PACKAGE_TARNAME "${PACKAGE}")
set (PACKAGE_VERSION "${VERSION}")
set (PACKAGE_STRING "${PACKAGE} ${VERSION}")
string (TIMESTAMP PACKAGE_DATE "%Y-%m-%d")

if (FREEBSD OR DRAGONFLY)
	set (ARTWORKFLOW_MANDIR  man                        CACHE STRING "Installation directory for man pages")
	set (ARTWORKFLOW_MAN1DIR ${ARTWORKFLOW_MANDIR}/man1 CACHE STRING "Installation directory for man pages, section 1")
	set (ARTWORKFLOW_MAN7DIR ${ARTWORKFLOW_MANDIR}/man7 CACHE STRING "Installation directory for man pages, section 7")
else (FREEBSD OR DRAGONFLY)
	set (ARTWORKFLOW_MANDIR  share/man                  CACHE STRING "Installation directory for man pages")
	set (ARTWORKFLOW_MAN1DIR ${ARTWORKFLOW_MANDIR}/man1 CACHE STRING "Installation directory for man pages, section 1")
	set (ARTWORKFLOW_MAN7DIR ${ARTWORKFLOW_MANDIR}/man7 CACHE STRING "Installation directory for man pages, section 7")
endif (FREEBSD OR DRAGONFLY)
set (ARTWORKFLOW_DOCDIR share/doc/artworkflow CACHE STRING "Installation directory for doc files")
set (ARTWORKFLOW_BINDIR bin                   CACHE STRING "Installation directory for Timewarrior executable")

check_function_exists(strlcpy HAVE_STRLCPY)
check_function_exists(timegm  HAVE_TIMEGM)

message ("-- Configuring cmake.h")
configure_file (${CMAKE_SOURCE_DIR}/cmake.h.in
                ${CMAKE_SOURCE_DIR}/cmake.h)

add_subdirectory (src)
add_subdirectory (src/commands)
add_subdirectory (src/libshared)
add_subdirectory (doc)
if (EXISTS ${CMAKE_SOURCE_DIR}/test)
  add_subdirectory (test EXCLUDE_FROM_ALL)
endif (EXISTS ${CMAKE_SOURCE_DIR}/test)

set (doc_FILES ChangeLog README.md INSTALL AUTHORS LICENSE)
foreach (doc_FILE ${doc_FILES})
	install (FILES ${doc_FILE} DESTINATION ${ARTWORKFLOW_DOCDIR})
endforeach (doc_FILE)

# ---

set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION})
set (CPACK_SOURCE_IGNORE_FILES  "CMakeCache"
                                "CMakeFiles"
                                "CPackConfig"
                                "CPackSourceConfig"
                                "_CPack_Packages"
                                "cmake_install"
                                "cmake.h$"
                                "commit.h$"
                                "install_manifest"
                                "Makefile$"
                                "package-config"
                                "src/artworkflow$"
                                "/\\\\.pre-commit-config.yaml"
                                "/\\\\.gitignore"
                                "/\\\\.git/"
                                "swp$")
include (CPack)
