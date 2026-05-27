cmake_minimum_required (VERSION 3.10)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src/libshared/src)

set (shared_SRCS Args.cpp          Args.h
                 Color.cpp         Color.h
		 Composite.cpp     Composite.h
		 Configuration.cpp Configuration.h
		 Datetime.cpp      Datetime.h
		 Duration.cpp      Duration.h
		 FS.cpp            FS.h
		 JSON.cpp          JSON.h
		 Lexer.cpp         Lexer.h
		 Log.cpp           Log.h
		 Msg.cpp           Msg.h
                 Packrat.cpp       Packrat.h
                 Palette.cpp       Palette.h
		 PEG.cpp           PEG.h
                 Pig.cpp           Pig.h
		 RX.cpp            RX.h
                 SAX.cpp
                 Table.cpp         Table.h
                 Timer.cpp         Timer.h
                 Tree.cpp          Tree.h
                 format.cpp        format.h
                 ip.cpp
                 shared.cpp        shared.h
                 unicode.cpp       unicode.h
                 utf8.cpp          utf8.h
		                   wcwidth.h)

add_library (shared STATIC ${shared_SRCS})

add_executable (lex_executable lex.cpp)
target_link_libraries (lex_executable shared)
set_property (TARGET lex_executable PROPERTY OUTPUT_NAME "lex")
