
cmake_minimum_required( VERSION 2.6 )
project( configParserLib ) 

set( configparser_source_files 
      cmdargs.cpp
      parser.cpp
      pythonparser.cpp
      simpleparser.cpp
      mutex.cpp
   )

include( FindPythonLibs )
find_package( PythonLibs REQUIRED )


add_library( configParser ${LIBRARY_TYPE} ${configparser_source_files} )
target_link_libraries( configParser ${PYTHON_LIBRARIES} )

