
cmake_minimum_required( VERSION 2.6 )
project( selfTest ) 

set ( interp_source_files
        interpreter.cpp
    )

add_executable( interpreter ${interp_source_files} )
target_link_libraries( interpreter configParserStruct )
target_include_directories( interpreter PRIVATE "${PROJECT_SOURCE_DIR}/.." )

