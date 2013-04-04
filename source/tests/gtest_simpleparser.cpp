
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>

#include "configParserStruct/simpleparser.h"

using namespace configParserStruct;

// =========================================================

TEST( simpleParser, simpleExec )
{
  const std::string TestProgram = 
    "x = 3+1;"
    "";

  simpleParser Parser;
  Parser.exec(TestProgram);
}

// ---------------------------------------------------------

TEST( simpleParser, stripSpaces )
{
  EXPECT_EQ( "abc", simpleParser::stripSpaces( " abc  \t " ) );
  EXPECT_EQ( "abc", simpleParser::stripSpaces( "abc  \t " ) );
  EXPECT_EQ( "abc", simpleParser::stripSpaces( " \n abc" ) );
}

// ---------------------------------------------------------

TEST( simpleParser, splitLineOnNameandValue )
{
  EXPECT_EQ( "a", simpleParser::splitLineOnNameandValue("a=1").first );
  EXPECT_EQ( "1", simpleParser::splitLineOnNameandValue("a=1").second );
  
  EXPECT_EQ( "Var",  simpleParser::splitLineOnNameandValue("Var = 12.3 # Comment").first );
  EXPECT_EQ( "12.3", simpleParser::splitLineOnNameandValue("Var = 12.3 # Comment").second );
  
  EXPECT_EQ( "", simpleParser::splitLineOnNameandValue("#Var = 12.3 # Comment").first );
  EXPECT_EQ( "", simpleParser::splitLineOnNameandValue("#Var = 12.3 # Comment").second );
}

// ---------------------------------------------------------

TEST( simpleParser, isVariableExist )
{
  simpleParser Parser;

  Parser.setVariableValue( "a", 1 );
  Parser.setVariableValue( "b", "xyz" );
  Parser.exec( "c = 2; d = 4;" );

  EXPECT_TRUE( Parser.isVariableExist("a") );
  EXPECT_TRUE( Parser.isVariableExist("b") );
  EXPECT_TRUE( Parser.isVariableExist("c") );
  EXPECT_TRUE( Parser.isVariableExist("d") );
  EXPECT_FALSE( Parser.isVariableExist("x") );
}

// ---------------------------------------------------------

TEST( simpleParser, stringVariable )
{
  simpleParser Parser;

  Parser.setVariableValue( "a", 1 );
  Parser.setVariableValue( "b", "xyz" );
  Parser.exec( "c = 2; ddd = 4;\ne.x = 1\ne.y = 2" );

  EXPECT_EQ( "1",   Parser.stringVariable("a") );
  EXPECT_EQ( "xyz", Parser.stringVariable("b") );
  EXPECT_EQ( "4",   Parser.stringVariable("ddd") );
  EXPECT_EQ( "1",   Parser.stringVariable("e.x") );
  EXPECT_EQ( "2",   Parser.stringVariable("e.y") );
  EXPECT_EQ( "",    Parser.stringVariable("x") );
  EXPECT_EQ( "X",   Parser.stringVariable("x","X") );
}

// ---------------------------------------------------------

TEST( simpleParser, numericVariable )
{
  simpleParser Parser( "a = 1; b = abc;" );
  
  EXPECT_EQ( 1,   Parser.integerVariable("a") );
  EXPECT_NEAR( 1, Parser.doubleVariable("a"), 1e-5 );
  EXPECT_EQ( 0,   Parser.integerVariable("b") );
  EXPECT_EQ( "abc", Parser.stringVariable("b") );
}

// ---------------------------------------------------------

TEST( simpleParser, listOfVariables )
{
  simpleParser Parser( "a = 1; b = 2; b = 3\n" );
  
  simpleParser::containerForVariables List = Parser.listOfVariables();

  for ( parser::containerForVariables::const_iterator i = List.begin(); i != List.end(); ++i )
    std::cerr << *i << std::endl;

  EXPECT_EQ( 2, List.size() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("a"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("b"))  != List.end() );
  EXPECT_FALSE( std::find(List.begin(),List.end(),std::string("c")) != List.end() );
}

// =========================================================


