
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>

#include "configParserStruct/pythonparser.h"

using namespace configParserStruct;

// =========================================================

TEST( pythonParser, simpleExec )
{
  const std::string TestProgram = 
    "x = 3+1;"
    "";

  pythonParser Parser;
  Parser.exec(TestProgram);
}

// ---------------------------------------------------------

TEST( pythonParser, isVariableExist )
{
  const std::string TestProgram = 
    "x = 1\n"
    "y = 2\n";

  pythonParser Parser;
  Parser.exec(TestProgram);

  ASSERT_TRUE( Parser.isVariableExist("x") );
  ASSERT_TRUE( Parser.isVariableExist("y") );
  ASSERT_FALSE( Parser.isVariableExist("z") );
}

// ---------------------------------------------------------

TEST( pythonParser, multipleParsers )
{
  const std::string TestProgram = 
    "x = 1\n"
    "y = 2\n";

  pythonParser Parser1( "x = 0; y = 1;" );
  pythonParser Parser2( "x = 2; z = 3;" );
  
  ASSERT_TRUE( Parser1.isVariableExist("x") );
  ASSERT_TRUE( Parser1.isVariableExist("y") );
  ASSERT_FALSE( Parser1.isVariableExist("z") );
  
  ASSERT_TRUE( Parser2.isVariableExist("x") );
  ASSERT_FALSE( Parser2.isVariableExist("y") );
  ASSERT_TRUE( Parser2.isVariableExist("z") );
}

// ---------------------------------------------------------

TEST( pythonParser, stringVariable )
{
  const std::string TestProgram = 
    "x = 1\n"
    "y = 'abc'\n";

  pythonParser Parser(TestProgram);

  ASSERT_EQ( "abc", Parser.stringVariable("y") );
  ASSERT_EQ( "1", Parser.stringVariable("x") );
  ASSERT_EQ( "", Parser.stringVariable("z") );
}

// ---------------------------------------------------------

TEST( pythonParser, numericVariable )
{
  const std::string TestProgram = 
    "x = 1\n"
    "y = x + 3\n"
    "z = 4.5\n";

  pythonParser Parser(TestProgram);

  ASSERT_EQ( 1,   Parser.integerVariable("x") );
  ASSERT_EQ( 4,   Parser.integerVariable("y") );
  ASSERT_NEAR( 4.5, Parser.doubleVariable("z"), 1e-5 );
  ASSERT_EQ( 4,   Parser.integerVariable("z") );
  ASSERT_EQ( 0,   Parser.integerVariable("a") );
}

// ---------------------------------------------------------

TEST( pythonParser, setVariableValue )
{
  const std::string TestProgram = 
    "y = x + 1\n"
    "";

  pythonParser Parser;
  Parser.setVariableValue( "x", 1 );
  Parser.setVariableValue( "z", "abc" );
  
  EXPECT_EQ( 1, Parser.integerVariable("x") );
  EXPECT_TRUE( Parser.isVariableExist("x") );
  EXPECT_EQ( std::string("abc"), Parser.stringVariable("z") );

  Parser.exec(TestProgram);
  
  EXPECT_EQ( 1, Parser.integerVariable("x") );
  EXPECT_EQ( 2, Parser.integerVariable("y") );
}

// ---------------------------------------------------------

TEST( pythonParser, listOfVariables )
{
  const std::string TestProgram = 
    "c = a + 1\n"
    "dd = 0.3\n"
    "e = 'abc'\n"
    "f = { 'k1' : 1, 'k2' : 'i2', 3 : 'i3' }\n"
    "";

  pythonParser Parser;
  Parser.setVariableValue( "a", 1 );
  Parser.setVariableValue( "b", "abc" );

  pythonParser::containerForVariables List;

  List = Parser.listOfVariables();
  EXPECT_EQ( 2, List.size() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("a"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("b"))  != List.end() );
  EXPECT_FALSE( std::find(List.begin(),List.end(),std::string("c")) != List.end() );
  
  Parser.exec(TestProgram);
  List = Parser.listOfVariables();

#if 0
  for ( pythonParser::containerForVariables::const_iterator i = List.begin(); i != List.end(); ++i )
    std::cerr << *i << std::endl;
#endif

  EXPECT_TRUE( List.size() >= 5 );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("a"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("b"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("c"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("dd"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("e"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("f.k1"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("f.k2"))  != List.end() );
  EXPECT_TRUE( std::find(List.begin(),List.end(),std::string("f.3"))  != List.end() );
  EXPECT_FALSE( std::find(List.begin(),List.end(),std::string("z")) != List.end() );
}

// ---------------------------------------------------------

TEST( pythonParser, listOfVariablesStruct )
{
  const std::string TestProgram = 
    "a = 0.3\n"
    "f = { 'k1' : 1, 'k2' : 'i2', 3 : 'i3' }\n"
    "ggg = { 'k3' : 0 }\n"
    "h = dict()\n"
    "";

  pythonParser Parser;
  pythonParser::containerForVariables List;
 
  Parser.exec(TestProgram);
  List = Parser.listOfVariablesStruct();
 
#if 0
  for ( pythonParser::containerForVariables::const_iterator i = List.begin(); i != List.end(); ++i )
    std::cerr << *i << std::endl;
#endif

  EXPECT_TRUE( List.size() >= 2 );
  EXPECT_TRUE(  std::find(List.begin(),List.end(),std::string("f"))  != List.end() );
  EXPECT_TRUE(  std::find(List.begin(),List.end(),std::string("ggg"))  != List.end() );
  EXPECT_FALSE( std::find(List.begin(),List.end(),std::string("h"))  != List.end() );
  EXPECT_FALSE( std::find(List.begin(),List.end(),std::string("a"))  != List.end() );
  EXPECT_FALSE( std::find(List.begin(),List.end(),std::string("i"))  != List.end() );
}

// ---------------------------------------------------------

TEST( pythonParser, incorrectProgram )
{
  const std::string TestProgram = 
    "a = 0.3\n"
    "b = xyz\n"
    "";

  pythonParser Parser;

  try
  {
    Parser.exec( TestProgram );
    FAIL() << "No exception!";
  } catch ( ... )
  {
  }
}

// =========================================================


