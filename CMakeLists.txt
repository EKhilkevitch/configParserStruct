
cmake_minimum_required( VERSION 2.6 )
project( graphics ) 

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set( EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/" )
set( LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib/" )

include_directories( "./source/" )

if ( NOT LIBRARY_TYPE )
  if ( WIN32 )
    set( LIBRARY_TYPE STATIC )
  else()
    set( LIBRARY_TYPE SHARED )
  endif()
endif()

message( "LIBRARY_TYPE = ${LIBRARY_TYPE}" )
message( "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )

if ( UNIX )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC") 
  set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC") 
endif()

add_subdirectory( ./source/configParserStruct/ )
add_subdirectory( ./source/tests/ )

if ( WIN32 )
  add_custom_command( COMMAND "sed -i 's/machine:[a-z0-9]*/machine:x86/g" TARGET Makefile )
endif()

