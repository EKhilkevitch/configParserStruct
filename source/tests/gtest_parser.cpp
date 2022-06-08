
// =========================================================

#include "configParserStruct/parser.h"
#include "configParserStruct/exception.h"

#include <iostream>
#include <cmath>

#include <gtest/gtest.h>

using namespace configParserStruct;

// =========================================================

TEST( parser, empty )
{
  parser Parser;
  Parser.exec( ";" );

  EXPECT_EQ( parser::SuccesssErrorLine, Parser.errorLine() );
  EXPECT_EQ( 0, Parser.doubleVariable( parser::LastExpressionValueName, 0 ) );
}

// ---------------------------------------------------------

TEST( parser, xxxVariable )
{
  parser Parser;
  Parser.exec( "a = 3 * 4; b = a + 1; c = exp(2.5);" );

  EXPECT_EQ( parser::SuccesssErrorLine, Parser.errorLine() );
  EXPECT_EQ( 12, Parser.integerVariable("a",0) );
  EXPECT_EQ( 13, Parser.integerVariable("b",0) );
  EXPECT_EQ( std::exp(2.5), Parser.doubleVariable("c",0) );
  EXPECT_EQ( std::exp(2.5), Parser.doubleVariable( parser::LastExpressionValueName, 0 ) );
}

// ---------------------------------------------------------

TEST( parser, xxxVariable_dict_1 )
{
  parser Parser;
  Parser.exec( "dict = { .a = 1, .b = 2, .c = 3 };" );

  EXPECT_EQ( parser::SuccesssErrorLine, Parser.errorLine() );
  EXPECT_EQ( 3,  Parser.integerVariable("dict") );
  EXPECT_EQ( 1, Parser.integerVariable("dict.a",0) );
  EXPECT_EQ( 2, Parser.integerVariable("dict.b",0) );
  EXPECT_EQ( 3, Parser.integerVariable("dict.c",0) );
  EXPECT_EQ( 0, Parser.integerVariable("dict.d",0) );
  EXPECT_EQ( 0, Parser.integerVariable("xxx.d",0) );
}

// ---------------------------------------------------------

TEST( parser, xxxVariable_dict_2 )
{
  parser Parser;
  Parser.exec( "dict = { .a = 1, .b = { .x = 5, .y = 6 }, .c = 3 };" );

  EXPECT_EQ( parser::SuccesssErrorLine, Parser.errorLine() );
  EXPECT_EQ( 3,  Parser.integerVariable("dict") );
  EXPECT_EQ( 1, Parser.integerVariable("dict.a",0) );
  EXPECT_EQ( 5, Parser.integerVariable("dict.b.x",0) );
  EXPECT_EQ( 6, Parser.integerVariable("dict.b.y",0) );
  EXPECT_EQ( 3, Parser.integerVariable("dict.c",0) );
  EXPECT_EQ( 0, Parser.integerVariable("dict.d",0) );
  EXPECT_EQ( 9, Parser.integerVariable("dict.d",9) );
  EXPECT_EQ( 0, Parser.integerVariable("xxx.d",0) );
}

// ---------------------------------------------------------

TEST( parser, dictKeys )
{
  parser Parser;
  Parser.exec( "dict = { .a = 1, .b = { .x = 5, .y = 6 }, .c = 3 };" );

  std::set<std::string> DictKeys = Parser.dictKeys( "dict" );
  EXPECT_EQ( 3, DictKeys.size() );
  EXPECT_EQ( 1, DictKeys.count("a") );
  EXPECT_EQ( 1, DictKeys.count("b") );
  EXPECT_EQ( 1, DictKeys.count("c") );
  
  DictKeys = Parser.dictKeys( "dict.b" );
  EXPECT_EQ( 2, DictKeys.size() );
  EXPECT_EQ( 1, DictKeys.count("x") );
  EXPECT_EQ( 1, DictKeys.count("y") );
}

// ---------------------------------------------------------

TEST( parser, isVariableExist )
{
  parser Parser;
  Parser.exec( "a = 3 * 4; b = a + 1; c = exp(2.5);" );

  EXPECT_TRUE( Parser.isVariableExist("a") );
  EXPECT_TRUE( Parser.isVariableExist("c") );
  EXPECT_FALSE( Parser.isVariableExist("d") );
}

// ---------------------------------------------------------

TEST( parser, variableType )
{
  parser Parser;
  Parser.exec( "a = 3 * 4; b = a + 1; c = exp(2.5);" );

  EXPECT_EQ( parser::VarInteger, Parser.variableType("a") );
  EXPECT_EQ( parser::VarInteger, Parser.variableType("b") );
  EXPECT_EQ( parser::VarReal, Parser.variableType("c") );
  EXPECT_EQ( parser::VarNone, Parser.variableType("d") );
  EXPECT_EQ( parser::VarReal, Parser.variableType( parser::LastExpressionValueName ) );
}

// ---------------------------------------------------------

TEST( parser, variables )
{
  parser Parser;
  Parser.build( "a = 3 * 4; b = a + 1; c = exp(2.5);" );

  const std::set<std::string> Variables = Parser.variables();

  EXPECT_EQ( 1, Variables.count("a") );
  EXPECT_EQ( 1, Variables.count("b") );
  EXPECT_EQ( 1, Variables.count("c") );
  EXPECT_EQ( 0, Variables.count("d") );
  EXPECT_EQ( 3, Variables.size() );
}

// ---------------------------------------------------------

TEST( parser, fail )
{
  parser Parser;

  try
  {
    Parser.build( "a = 3 * 4;\nb = a + 1;\nc = exp(2.5) +;\nd = 3;" );
    FAIL() << "No exception";
  } catch ( const exception &Exception ) {
    EXPECT_EQ( 3, Parser.errorLine() );
  }
}

// ---------------------------------------------------------

TEST( parser, setVariable_before )
{
  parser Parser;
  Parser.setVariable( "a", 3 );
  Parser.setVariable( "b", 4.5 );
  Parser.setVariable( "z", 1 );
  Parser.build( "a = 3 * 4; b = b + 1; c = exp(2.5);" );
  Parser.run();
  
  EXPECT_EQ( 12, Parser.integerVariable("a",0) );
  EXPECT_EQ( 5.5, Parser.doubleVariable("b",0) );
  EXPECT_EQ( 1, Parser.integerVariable("z",0) );
}

// ---------------------------------------------------------

TEST( parser, setVariable_afterBuild )
{
  parser Parser;
  Parser.build( "a = 3 * 4; b = b + 1; c = exp(2.5);" );
  Parser.setVariable( "a", 3 );
  Parser.setVariable( "b", 4.5 );
  Parser.setVariable( "z", 1 );
  Parser.run();
  
  EXPECT_EQ( 12, Parser.integerVariable("a",0) );
  EXPECT_EQ( 5.5, Parser.doubleVariable("b",0) );
  EXPECT_EQ( 1, Parser.integerVariable("z",0) );
}

// ---------------------------------------------------------

TEST( parser, setVariable_afterRun )
{
  parser Parser;
  Parser.build( "a = 3 * 4; b = b + 1; c = exp(2.5);" );
  Parser.setVariable( "a", 3 );
  Parser.setVariable( "b", 4.5 );
  Parser.setVariable( "z", 1 );
  Parser.run();
  Parser.setVariable( "z", 2 );
  
  EXPECT_EQ( 12, Parser.integerVariable("a",0) );
  EXPECT_EQ( 5.5, Parser.doubleVariable("b",0) );
  EXPECT_EQ( 2, Parser.integerVariable("z",0) );
}

// ---------------------------------------------------------

TEST( parser, run_twice )
{
  parser Parser;
  Parser.setVariable( "b", 4 );
  EXPECT_EQ( 4, Parser.integerVariable("b",0) );

  Parser.exec( "b = b + 1;" );
  EXPECT_EQ( 5, Parser.integerVariable("b",0) );

  Parser.exec( "b = b + 1;" );
  EXPECT_EQ( 5, Parser.integerVariable("b",0) );
}

// ---------------------------------------------------------

TEST( parser, copy_1 )
{
  parser Parser1;
  Parser1.build( "a = 3 * 4; b = b + 1; c = exp(2.5);" );
  Parser1.setVariable( "a", 3 );
  Parser1.setVariable( "b", 4.5 );
  Parser1.setVariable( "z", 1 );

  parser Parser2 = Parser1;
  Parser1.run();
  Parser2.run();
  
  EXPECT_EQ( 12, Parser1.integerVariable("a",0) );
  EXPECT_EQ( 5.5, Parser1.doubleVariable("b",0) );
  EXPECT_EQ( 1, Parser1.integerVariable("z",0) );
  
  EXPECT_EQ( 12, Parser2.integerVariable("a",0) );
  EXPECT_EQ( 5.5, Parser2.doubleVariable("b",0) );
  EXPECT_EQ( 1, Parser2.integerVariable("z",0) );
}

// ---------------------------------------------------------

TEST( parser, copy_2 )
{
  parser Parser1;
  Parser1.build( "a = 3 * 4; b = b + 1; c = exp(2.5);" );
  Parser1.setVariable( "a", 3 );
  Parser1.setVariable( "b", 4.5 );
  Parser1.setVariable( "z", 1 );

  parser Parser2 = Parser1;
  Parser2.build( "a = a + 1;" );
  Parser1.run();
  Parser2.run();
  
  EXPECT_EQ( 12, Parser1.integerVariable("a",0) );
  EXPECT_EQ( 5.5, Parser1.doubleVariable("b",0) );
  EXPECT_EQ( 1, Parser1.integerVariable("z",0) );
  
  EXPECT_EQ( 4, Parser2.integerVariable("a",0) );
  EXPECT_EQ( 4.5, Parser2.doubleVariable("b",0) );
  EXPECT_EQ( 1, Parser2.integerVariable("z",0) );
}

// ---------------------------------------------------------

TEST( parser, xxxVariable_last )
{
  parser Parser;
  Parser.build( "t < 0 ? 0 : A*exp(-t/s);" );
  Parser.setVariable( "s", 3.0 );
  Parser.setVariable( "A", 2.0 );
//  std::cerr << "first:" << std::endl;
//  std::cerr << Parser.toDebugString() << std::endl;
  Parser.run();
  Parser.setVariable( "t", 0.4 );
//  std::cerr << "second:" << std::endl;
//  std::cerr << Parser.toDebugString() << std::endl;
  Parser.run();

  EXPECT_EQ( 2*std::exp(-0.4/3.0), Parser.doubleVariable( parser::LastExpressionValueName ) ) << Parser.toDebugString();
}

// =========================================================

