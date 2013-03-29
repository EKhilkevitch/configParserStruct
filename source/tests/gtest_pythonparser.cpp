
// =========================================================

#include <gtest/gtest.h>
#include <cstdio>

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
  
  EXPECT_EQ( 1, Parser.integerVariable("x") );
  EXPECT_TRUE( Parser.isVariableExist("x") );

  Parser.exec(TestProgram);
  
  EXPECT_EQ( 1, Parser.integerVariable("x") );
  EXPECT_EQ( 2, Parser.integerVariable("y") );
}

// =========================================================


