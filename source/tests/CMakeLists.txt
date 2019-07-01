
cmake_minimum_required( VERSION 2.6 )

if ( ENABLE_TESTS )

  project( selfTestconfigParserStruct ) 

  set ( tests_source_files
          gtest_stringcast.cpp
          gtest_cmdargs.cpp
          gtest_parser.cpp
          gtest_simpleparser.cpp
          gtest_structparser.cpp
          gtest_structparserinput.cpp
          gtest_structparsercommand.cpp
          gtest_structparservars.cpp
          gtest_structparserprogram.cpp
      )

  include( FindPythonLibs )
  find_package( PythonLibs )
  if ( PYTHON_INCLUDE_DIRS )
    set( tests_source_files ${tests_source_files} gtest_pythonparser.cpp )
  endif()

  find_library( gtest  NAMES gtest )
  find_library( gtest_main  NAMES gtest_main )

  if ( NOT ${gtest} MATCHES gtest-NOTFOUND )
    message( "Tests enabled" )
    add_executable( selfTest-configParserStruct ${tests_source_files} )
    target_link_libraries( selfTest-configParserStruct ${gtest} )
    target_link_libraries( selfTest-configParserStruct ${gtest_main} )
    target_link_libraries( selfTest-configParserStruct configParserStruct )
    target_link_libraries( selfTest-configParserStruct pthread )
    target_include_directories( selfTest-configParserStruct PRIVATE "${PROJECT_SOURCE_DIR}/.." )
  else()
    message( "gtest not found, test not build" )    
  endif()
else()
  message( "Tests disabed (use -DENABLE_TESTS=1)" )
endif()  

