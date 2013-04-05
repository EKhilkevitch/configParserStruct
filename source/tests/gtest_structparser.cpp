
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>

#include "configParserStruct/structparser.h"

using namespace configParserStruct;

// =========================================================

TEST( structParser, structParser )
{
  const std::string TestProgram = 
    "x = 3+1;\n"
    "";

  structParser Parser;
  Parser.exec(TestProgram);
}

// ---------------------------------------------------------

TEST( structParser, numericVariable )
{
  const std::string TestProgram = 
    "x = 3; y = 2;"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  EXPECT_TRUE( Parser.isVariableExist("x") );
  EXPECT_EQ( 3, Parser.integerVariable("x") );

  EXPECT_TRUE( Parser.isVariableExist("y") );
  EXPECT_EQ( 2, Parser.integerVariable("y") );
  
  EXPECT_FALSE( Parser.isVariableExist("z") );
  EXPECT_EQ( 0, Parser.integerVariable("z") );
}

// ---------------------------------------------------------

TEST( structParser, add )
{
  const std::string TestProgram = 
    "x = 3; y = 2 + x; z = 9 + 10 + x"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  EXPECT_EQ(  3, Parser.integerVariable("x") );
  EXPECT_EQ(  5, Parser.integerVariable("y") );
  EXPECT_EQ( 22, Parser.integerVariable("z") );
}

// ---------------------------------------------------------

TEST( structParser, sub )
{
  const std::string TestProgram = 
    "x = 3; y = 2 - x; z = 10 - 9 - 3"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  EXPECT_EQ(  3, Parser.integerVariable("x") );
  EXPECT_EQ( -1, Parser.integerVariable("y") );
  EXPECT_EQ( -2, Parser.integerVariable("z") );
}

// =========================================================


