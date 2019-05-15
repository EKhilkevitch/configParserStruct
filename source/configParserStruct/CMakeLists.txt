
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
endif()

if ( UNIX )
  set( BISON_FLAGS "--name-prefix=CPSSPU_ -v" )
  set( FLAX_FLAGS "--prefix=CPSSPU_" )
endif()


set( configparser_source_files 
      cmdargs.cpp
      parser.cpp
      exception.cpp
      simpleparser.cpp
      mutex.cpp
   )

if ( WIN32 )
  set( USE_INTERNAL_GETOPT 1 )
  set( WIN_GETOPT_DIR "${PROJECT_SOURCE_DIR}/../../win_getopt" ) 
  set( configparser_source_files ${configparser_source_files}
       ${WIN_GETOPT_DIR}/getopt.c
       ${WIN_GETOPT_DIR}/getopt_long.c
       ${WIN_GETOPT_DIR}/libecho.c
     )
endif()


if ( FLEX_EXECUTABLE AND BISON_EXECUTABLE )
  BISON_TARGET( structparserY structparser.y  ${CMAKE_CURRENT_BINARY_DIR}/structparser.tab.c COMPILE_FLAGS ${BISON_FLAGS} )
  FLEX_TARGET(  structparserL structparser.l  ${CMAKE_CURRENT_BINARY_DIR}/structparser.yy.c  COMPILE_FLAGS ${FLAX_FLAGS} )
  ADD_FLEX_BISON_DEPENDENCY( structparserL structparserY )
  set( USE_BISON_FLEX 1 )
  set( configparser_source_files ${configparser_source_files}
      structparserinput.cpp
      structparsercompiler.cpp
      structparsercommand.cpp
      structparservars.cpp
      structparserbuiltin.cpp
      structparserextmath.cpp
      structparserprogram.cpp
      structparser.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/structparser.tab.c
      ${CMAKE_CURRENT_BINARY_DIR}/structparser.yy.c
     ) 
endif()

if ( PYTHON_INCLUDE_DIRS AND ENABLE_PYTHON )
  include( FindPythonLibs )
  find_package( PythonLibs )

  message( "Python ENABLED" )
  set( configparser_source_files ${configparser_source_files} pythonparser.cpp )
  set( USE_PYTHON 1 )

  if ( UNIX )
    set( PYTHON_VERSION ${PYTHON_INCLUDE_PATH} )
    string( REGEX REPLACE "^.*ython" "" PYTHON_VERSION "${PYTHON_VERSION}"  )
    string( REGEX REPLACE "[\\/\\\\]+.*$" "" PYTHON_VERSION "${PYTHON_VERSION}"  )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} '-DPYTHON_INCLUDE_HEADER=<python${PYTHON_VERSION}/Python.h>'") 
  endif()
  if ( WIN32 )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPYTHON_INCLUDE_HEADER=<Python.h>") 
  endif()

endif()

add_library( configParserStruct ${LIBRARY_TYPE} ${configparser_source_files} )

if ( USE_INTERNAL_GETOPT )
  target_include_directories( configParserStruct PRIVATE ${WIN_GETOPT_DIR} )
endif()

if ( USE_BISON_FLEX )
  target_include_directories( configParserStruct PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/.." )
endif()

if ( USE_PYTHON )
  if ( UNIX )
    target_include_directories( configParserStruct PRIVATE "${PYTHON_INCLUDE_PATH}/.." )
  endif()
  if ( WIN32 )
    target_include_directories( configParserStruct PRIVATE "${PYTHON_INCLUDE_PATH}" )
  endif()
endif()

if ( PYTHON_INCLUDE_DIRS )
  target_link_libraries( configParserStruct ${PYTHON_LIBRARIES} )
endif()

