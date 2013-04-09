
cmake_minimum_required( VERSION 2.6 )
project( configParserLib ) 

find_package( BISON )
find_package( FLEX )

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
      simpleparser.cpp
      mutex.cpp
   )

if ( WIN32 )
  set( WIN_GETOPT_DIR "${PROJECT_SOURCE_DIR}/../../win_getopt" ) 
  include_directories( ${WIN_GETOPT_DIR} )
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
  include_directories( "${CMAKE_CURRENT_BINARY_DIR}/.." )
  set( configparser_source_files ${configparser_source_files}
      structparserutil.cpp
      structparserinput.cpp
      structparsercompiler.cpp
      structparsercommand.cpp
      structparservars.cpp
      structparserbuiltin.cpp
      structparserprogram.cpp
      structparser.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/structparser.tab.c
      ${CMAKE_CURRENT_BINARY_DIR}/structparser.yy.c
     ) 
endif()

include( FindPythonLibs )
find_package( PythonLibs )
if ( PYTHON_INCLUDE_DIRS )

  set( configparser_source_files ${configparser_source_files} pythonparser.cpp )
  
    if ( UNIX )
    include_directories( "${PYTHON_INCLUDE_PATH}/.." )
    set( PYTHON_VERSION ${PYTHON_INCLUDE_PATH} )
    string( REGEX REPLACE "^.*ython" "" PYTHON_VERSION "${PYTHON_VERSION}"  )
    string( REGEX REPLACE "[\\/\\\\]+.*$" "" PYTHON_VERSION "${PYTHON_VERSION}"  )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} '-DPYTHON_INCLUDE_HEADER=<python${PYTHON_VERSION}/Python.h>'") 
  endif()
  if ( WIN32 )
    include_directories( "${PYTHON_INCLUDE_PATH}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPYTHON_INCLUDE_HEADER=<Python.h>") 
  endif()

endif()

if ( WIN32 )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS=1" )
  set( CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS=1" )
endif()

add_library( configParserStruct ${LIBRARY_TYPE} ${configparser_source_files} )

if ( PYTHON_INCLUDE_DIRS )
  target_link_libraries( configParserStruct ${PYTHON_LIBRARIES} )
endif()

