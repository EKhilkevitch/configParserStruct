
cmake_minimum_required( VERSION 2.6 )
project( selfTest ) 

set ( tests_source_files
        gtest_stringcast.cpp
        gtest_cmdargs.cpp
        gtest_parser.cpp
        gtest_pythonparser.cpp
        gtest_simpleparser.cpp
        gtest_structparser.cpp
        gtest_structparserinput.cpp
        gtest_structparsercommand.cpp
        gtest_structparservars.cpp
        gtest_structparserprogram.cpp
    )

find_library( gtest  NAMES gtest )
find_library( gtest_main  NAMES gtest_main )

if ( NOT ${gtest} MATCHES gtest-NOTFOUND AND ENABLE_TESTS )

  find_program( ICPC_PROGRAM icpc )
  if ( ICPC_PROGRAM )
    set( CMAKE_CXX_COMPILER icpc ) 
    set( CMAKE_C_COMPILER icc ) 
  endif()

  message( "Tests enabled" )
  add_executable( selfTest ${tests_source_files} )
  target_link_libraries( selfTest ${gtest} )
  target_link_libraries( selfTest ${gtest_main} )
  target_link_libraries( selfTest configParserStruct )
  target_link_libraries( selfTest pthread )
  set_target_properties( selfTest PROPERTIES COMPILE_FLAGS "-std=c++0x")
else()
  message( "Tests disabed" )
endif()  

