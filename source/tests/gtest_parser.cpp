
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <vector>

#include "configParserStruct/parser.h"

using namespace configParserStruct;

// =========================================================

template <class T> static std::vector<T> listToVector( const std::list<T> &List )
{
  return std::vector<T>( List.begin(), List.end() );
}

// =========================================================

TEST( parser, joinStringList )
{
  std::list<std::string> List;
  List.push_back( "abc" );
  List.push_back( "def" );
  List.push_back( "ghi" );

  std::string String = parser::joinStringList( List, "!" );
  ASSERT_EQ( std::string("abc!def!ghi!"), String );
}

// ---------------------------------------------------------

TEST( parser, splitString )
{
  std::vector<std::string> Result = listToVector( parser::splitString( "abc.def.efg.h", "." ) );
  ASSERT_EQ( 4, Result.size() );
  EXPECT_EQ( "abc", Result.at(0) );
  EXPECT_EQ( "def", Result.at(1) );
  EXPECT_EQ( "efg", Result.at(2) );
  EXPECT_EQ( "h", Result.at(3) );

  Result = listToVector( parser::splitString( "xyz", "." ) );
  ASSERT_EQ( 1, Result.size() );
  EXPECT_EQ( "xyz", Result.at(0) );

  Result = listToVector( parser::splitString( "ab;cd,ef.gh", ".,;" ) );
  ASSERT_EQ( 4, Result.size() );
  EXPECT_EQ( "ab", Result.at(0) );
  EXPECT_EQ( "cd", Result.at(1) );
  EXPECT_EQ( "ef", Result.at(2) );
  EXPECT_EQ( "gh", Result.at(3) );
  
  Result = listToVector( parser::splitString( ".abc.d.", "." ) );
  ASSERT_EQ( 4, Result.size() );
  EXPECT_EQ( "", Result.at(0) );
  EXPECT_EQ( "abc", Result.at(1) );
  EXPECT_EQ( "d", Result.at(2) );
  EXPECT_EQ( "", Result.at(3) );
}

// ---------------------------------------------------------

TEST( parser, readFileContent )
{
  const std::string TestFileName = "/tmp/test_readFileContent.txt";

  FILE *File = std::fopen( TestFileName.c_str(), "w" );
  if ( File == NULL )
    FAIL() << "Can not open test file";
  std::fprintf(File,"abc\n");
  std::fprintf(File,"def\n");
  std::fprintf(File,"ghi\n");
  std::fclose(File);

  std::list<std::string> Content = parser::readFileContent(TestFileName);
  ASSERT_EQ( 3, Content.size() );

  std::string ContentString = parser::joinStringList( Content, "" );
  ASSERT_EQ( "abc\ndef\nghi\n", ContentString );

  std::remove( TestFileName.c_str() );
}

// =========================================================


