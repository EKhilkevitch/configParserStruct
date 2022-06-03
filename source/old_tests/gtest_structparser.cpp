
// =========================================================

#include <gtest/gtest.h>

#include <cmath>
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
    "x = 3; y = 2+3;"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  EXPECT_TRUE( Parser.isVariableExist("x") );
  EXPECT_EQ( 3, Parser.integerVariable("x") );

  EXPECT_TRUE( Parser.isVariableExist("y") );
  EXPECT_EQ( 5, Parser.integerVariable("y") );
  
  EXPECT_FALSE( Parser.isVariableExist("z") );
  EXPECT_EQ( 0, Parser.integerVariable("z") );
}

// ---------------------------------------------------------

TEST( structParser, structVariable )
{
  const std::string TestProgram = 
    "x = 3; y = { .a = 4, .b = 2*x };"
    "";

  structParser Parser;
  Parser.exec(TestProgram);
  
  EXPECT_EQ( 3, Parser.integerVariable("x") );
  EXPECT_EQ( 4, Parser.integerVariable("y.a") );
  EXPECT_EQ( 6, Parser.integerVariable("y.b") );
}

// ---------------------------------------------------------

TEST( structParser, setVariableValue )
{
  structParser Parser;

  Parser.setVariableValue( "a", 1 );
  Parser.setVariableValue( "b", 1.3 );
  Parser.setVariableValue( "c", "xyz" );
  
  EXPECT_EQ( 1, Parser.integerVariable("a") );
  EXPECT_NEAR( 1.3, Parser.doubleVariable("b"), 1e-5 );
  EXPECT_EQ( "xyz", Parser.stringVariable("c") );
  
  Parser.exec("x = 2*a + b;"); 
  EXPECT_NEAR( 3.3, Parser.doubleVariable("x"), 1e-5 );
}

// ---------------------------------------------------------

TEST( structParser, setVariableValue_2 )
{
  structParser Parser;
  Parser.build( " x = 2*a + b + 2;" );
  
  EXPECT_NEAR( 0, Parser.doubleVariable("x"), 1e-5 );
  EXPECT_NEAR( 0, Parser.doubleVariable("a"), 1e-5 );
  EXPECT_NEAR( 0, Parser.doubleVariable("b"), 1e-5 );

  Parser.setVariableValue( "a", 3 );
  Parser.setVariableValue( "b", 5 );
  EXPECT_NEAR( 0, Parser.doubleVariable("x"), 1e-5 );
  EXPECT_NEAR( 3, Parser.doubleVariable("a"), 1e-5 );
  EXPECT_NEAR( 5, Parser.doubleVariable("b"), 1e-5 );

  Parser.run();
  EXPECT_NEAR( 13, Parser.doubleVariable("x"), 1e-5 );
  EXPECT_NEAR(  3, Parser.doubleVariable("a"), 1e-5 );
  EXPECT_NEAR(  5, Parser.doubleVariable("b"), 1e-5 );
  
  Parser.setVariableValue( "a", 10 );
  Parser.run();
  EXPECT_NEAR( 27, Parser.doubleVariable("x"), 1e-5 );
  EXPECT_NEAR( 10, Parser.doubleVariable("a"), 1e-5 );
  EXPECT_NEAR(  5, Parser.doubleVariable("b"), 1e-5 );
}

// ---------------------------------------------------------

TEST( structParser, builtinFunctions )
{
  const std::string TestProgram = 
    "x = exp(3); y = sin(4);"
    "";

  structParser Parser;
  Parser.exec(TestProgram);
  
  EXPECT_NEAR( std::exp(3), Parser.doubleVariable("x"), 1e-5 );
  EXPECT_NEAR( std::sin(4), Parser.doubleVariable("y"), 1e-5 );
}

// ---------------------------------------------------------

TEST( structParser, add )
{
  const std::string TestProgram = 
    "x = 3; y = 2 + x; z = 9 + 10 + x;"
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
    "x = 3; y = 2 - x; z = 10 - 9 - 3;"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  EXPECT_EQ(  3, Parser.integerVariable("x") );
  EXPECT_EQ( -1, Parser.integerVariable("y") );
  EXPECT_EQ( -2, Parser.integerVariable("z") );
}

// ---------------------------------------------------------

TEST( structParser, divZero )
{
  const std::string TestProgram = 
    "x = 0; y = 1; z = y/x;\n"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  EXPECT_EQ( 1.0/0.0, Parser.doubleVariable("z") );
}

// ---------------------------------------------------------

TEST( structParser, listOfVariables )
{
  const std::string TestProgram = 
    "x = 3; y = { .a = 2 }; z = c;\n"
    "v1 = x ? 1 : v2;\n"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  std::set<std::string> List = Parser.listOfVariables();

#if 0
  for ( std::set<std::string>::iterator i = List.begin(); i != List.end(); ++i )
    std::cout << *i << " " << std::endl;
#endif

  EXPECT_EQ( 1, List.count("x") );
  EXPECT_EQ( 1, List.count("c") );
  EXPECT_EQ( 1, List.count("y.a") );
  EXPECT_EQ( 1, List.count("y") );
  EXPECT_EQ( 1, List.count("z") );
  EXPECT_EQ( 1, List.count("v1") );
  EXPECT_EQ( 1, List.count("v2") );

}

// ---------------------------------------------------------

TEST( structParser, listOfVariablesStruct )
{
  const std::string TestProgram = 
    "x = 3; y = { .a = 2 }; z = { .b = 3 };\n"
    "";

  structParser Parser;
  Parser.exec(TestProgram);

  std::set<std::string> List = Parser.listOfVariablesStruct();

#if 0
  for ( std::set<std::string>::iterator i = List.begin(); i != List.end(); ++i )
    std::cout << *i << " " << std::endl;
#endif

  EXPECT_EQ( 3, List.size() );
  EXPECT_EQ( 0, List.count("x") );
  EXPECT_EQ( 1, List.count("y") );
  EXPECT_EQ( 1, List.count("z") );
  EXPECT_EQ( 1, List.count("LAST_EXPRESSION::") );
}

// =========================================================


