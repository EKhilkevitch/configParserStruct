
cmake_minimum_required( VERSION 2.6 )
project( configParserLib ) 

set( configparser_source_files 
      cmdargs.cpp
      parser.cpp
   )

add_library( configParser ${LIBRARY_TYPE} ${configparser_source_files} )

