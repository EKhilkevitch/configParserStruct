
cmake_minimum_required( VERSION 2.6 )
project( configParserLib ) 

if ( WIN32 )
  set( CMAKE_SYSTEM_PROGRAM_PATH "${CMAKE_SYSTEM_PROGRAM_PATH}" "${PROJECT_SOURCE_DIR}/../../win_programs/" )
endif()

find_package( FLEX )
find_package( BISON )

if ( WIN32 )
  set( BISON_FLAGS "--name-prefix=CPSSPU_ -v" )
  set( FLAX_FLAGS "-PCPSSPU_" )
  message( "BISON_EXECUTABLE = ${BISON_EXECUTABLE}" )
  message( "FLEX_EXECUTABLE = ${FLEX_EXECUTABLE}" )
  if ( BISON_EXECUTABLE MATCHES "win_programs" OR BISON_EXECUTABLE MATCHES "unix/wbin/" )
    set( ENV{BISON_SIMPLE} "${PROJECT_SOURCE_DIR}/../../win_programs/bison.simple" )
    set( ENV{BISON_HAIRY} "${PROJECT_SOURCE_DIR}/../../win_programs/bison.hairy" )
    #message( "Set SIMPLE = $ENV{BISON_SIMPLE}, HAIRY = $ENV{BISON_HAIRY}" )
  endif()
endif()

if ( UNIX )
  set( BISON_FLAGS "--name-prefix=CPSSPU_ -v" )
  set( FLAX_FLAGS "--prefix=CPSSPU_" )
endif()

BISON_TARGET( parserY parser.y  ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c COMPILE_FLAGS ${BISON_FLAGS} )
FLEX_TARGET(  parserL parser.l  ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.c  COMPILE_FLAGS ${FLAX_FLAGS} )
ADD_FLEX_BISON_DEPENDENCY( parserL parserY )

set( configparser_source_files 
      parser.cpp
      exception.cpp
      mutex.cpp
     ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c
     ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.c
     variablevalue.cpp
     reference.cpp
     variable.cpp
     command.cpp
     commandbinops.cpp
     commandcmp.cpp
     commandstack.cpp
     commandjump.cpp
     buildinfunc.cpp
     text.cpp
     memory.cpp
     optimizer.cpp
     program.cpp
     lexinput.c
     bisoncompiler.cpp
   )

if ( UNIX )
  set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Wextra -pedantic -ansi -std=c89") 
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++98" )
endif()

if ( UNIX )
  set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs" )
endif()

add_library( configParserStruct ${LIBRARY_TYPE} ${configparser_source_files} )
target_include_directories( configParserStruct PRIVATE "${PROJECT_SOURCE_DIR}/.." )
target_include_directories( configParserStruct PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/.." )

if ( WIN32 )
  target_compile_definitions( configParserStruct PRIVATE "-D_CRT_SECURE_NO_DEPRECATE=1" )
endif()

if ( UNIX )
  target_compile_definitions( configParserStruct PRIVATE "-D_POSIX_C_SOURCE=1" )
endif()

