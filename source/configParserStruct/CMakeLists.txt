
cmake_minimum_required( VERSION 2.6 )
project( configParserLib ) 

find_package(BISON)
find_package(FLEX)

BISON_TARGET( structparserY structparser.y  ${CMAKE_CURRENT_BINARY_DIR}/structparser.tab.c COMPILE_FLAGS "--name-prefix=strprs_" )
FLEX_TARGET(  structparserL structparser.l  ${CMAKE_CURRENT_BINARY_DIR}/structparser.yy.c  COMPILE_FLAGS "--prefix=strprs_" )
ADD_FLEX_BISON_DEPENDENCY( structparserL structparserY )
include_directories( "${CMAKE_CURRENT_BINARY_DIR}/.." )

set( configparser_source_files 
      cmdargs.cpp
      parser.cpp
      pythonparser.cpp
      simpleparser.cpp
      mutex.cpp
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

include( FindPythonLibs )
find_package( PythonLibs REQUIRED )

include_directories( "${PYTHON_INCLUDE_PATH}/.." )
set( PYTHON_VERSION ${PYTHON_INCLUDE_PATH} )
string( REGEX REPLACE "^.*ython" "" PYTHON_VERSION "${PYTHON_VERSION}"  )
string( REGEX REPLACE "[\\/]+$" "" PYTHON_VERSION "${PYTHON_VERSION}"  )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} '-DPYTHON_INCLUDE_HEADER=<python${PYTHON_VERSION}/Python.h>'") 

add_library( configParserStruct ${LIBRARY_TYPE} ${configparser_source_files} )
target_link_libraries( configParserStruct ${PYTHON_LIBRARIES} )

