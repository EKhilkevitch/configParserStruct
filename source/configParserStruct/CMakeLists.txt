
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
      structparser.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/structparser.tab.c
      ${CMAKE_CURRENT_BINARY_DIR}/structparser.yy.c
   )

include( FindPythonLibs )
find_package( PythonLibs REQUIRED )

include_directories( ${PYTHON_INCLUDE_PATH} )

add_library( configParser ${LIBRARY_TYPE} ${configparser_source_files} )
target_link_libraries( configParser ${PYTHON_LIBRARIES} )

