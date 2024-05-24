
cmake_minimum_required( VERSION 2.6 )
project( configParserStruct ) 

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set( EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/" )
set( LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib/" )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

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
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ") 
  set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Wextra ") 
endif()

if ( WIN32 )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ") 
  set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /wd4244 /wd4267 /wd4996 /wd4028") 
endif()

add_subdirectory( ./source/configParserStruct/ )
add_subdirectory( ./source/tests/ )
add_subdirectory( ./source/interpreter/ )

