
// =========================================================

#include <gtest/gtest.h>
#include <cstdio>

#include "configParserStruct/parser.h"

using namespace configParserStruct;

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


