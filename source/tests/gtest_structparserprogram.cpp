
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>

#include "configParserStruct/structparserprogram.h"

using namespace configParserStruct;
using namespace structParserUtil;

// =========================================================

TEST( program, empty )
{
  program Program;
  Program.build( "" );

  EXPECT_EQ( 0, Program.numberOfCommands() );
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 0, Program.variableNames().size() );

  Program.execute();
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 0, Program.variableNames().size() );
}

// ---------------------------------------------------------

TEST( program, number )
{
  program Program;
  Program.build( "1" );

  EXPECT_TRUE( Program.numberOfCommands() > 0 );
  
  Program.execute();
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 0, Program.variableNames().size() );
}

// ---------------------------------------------------------

TEST( program, clear )
{
  program Program;
  Program.build( "x = 1" );
  Program.execute();
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 1, Program.variableNames().size() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );

  Program.clear();
  EXPECT_EQ( 0, Program.numberOfCommands() );
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 0, Program.variableNames().size() );
}

// ---------------------------------------------------------

TEST( program, numericVariable )
{
  program Program;
  Program.build( "x = 1" );
  Program.execute();
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 1, Program.variableNames().size() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );

  Program.rebuildAndExecute( "x = y + 1" );
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 1, Program.variableNames().size() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 0,   Program.getNamedVariable("y").number(), 1e-5 );
  
  Program.rebuildAndExecute( "x = 3.3; y = 4e2; z = 0.01" );
  EXPECT_NEAR( 3.3,  Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 4e2,  Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( 0.01, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, stringVariable )
{
  program Program;
  Program.rebuildAndExecute( "x = 'abc'; y = \"zz\";" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 2, Program.variableNames().size() );
  EXPECT_EQ( "abc", Program.getNamedVariable("x").string() );
  EXPECT_EQ( "zz",  Program.getNamedVariable("y").string() );
}

// ---------------------------------------------------------

TEST( program, add )
{
  program Program;
  Program.rebuildAndExecute( "x = 1 + 2; y = 4+ x;" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 3, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 7, Program.getNamedVariable("y").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, sub )
{
  program Program;
  Program.rebuildAndExecute( "x = 5 - 2;" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 3, Program.getNamedVariable("x").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, mul )
{
  program Program;
  Program.rebuildAndExecute( "x = 5 * 2; y = 1 + 3*2; z = (1+3)*2;" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 10, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR(  7, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR(  8, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, div )
{
  program Program;
  Program.rebuildAndExecute( "x = 5 / 2; y = 1 + 2/4; z = (1+2)/3;" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 2.5, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 1.5, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( 1.0, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, unarySign )
{
  program Program;
  Program.rebuildAndExecute( "x = -2; y = +3; z = -3 -1;" );

  //std::cerr << Program.toString() << std::endl;
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR(  -2, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR(  +3, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR(  -4, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, braces )
{
  program Program;
  Program.rebuildAndExecute( "x = 2*3+1; y=2*(3+1);" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 7.0, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 8.0, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( 0.0, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, opEq )
{
  program Program;
  Program.rebuildAndExecute( "a = b = c = d = 5; a += 2; b -= 2; c *= 2; d /= 2;" );

//  std::cout << Program.toString();

  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 7.0, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_NEAR( 3.0, Program.getNamedVariable("b").number(), 1e-5 );
  EXPECT_NEAR( 10,  Program.getNamedVariable("c").number(), 1e-5 );
  EXPECT_NEAR( 2.5, Program.getNamedVariable("d").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, dict )
{
  program Program;
  Program.build( "a = 3; b = { .x = 0.1, .z = 2*a, .y = { .l = 'abcd' } }" );
  //std::cerr << Program.toString() << std::endl;
  Program.execute();
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 3.0, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_EQ( "{ .x = 0.1, .y = { .l = abcd }, .z = 6 }", Program.getNamedVariable("b").string() );
  try
  {
    dictVariableValue Struct = Program.getNamedVariable("b").value<dictVariableValue>();
    EXPECT_EQ( 3, Struct.numberOfItems() );
    EXPECT_NEAR( 0.1, Struct.getItem("x").number(), 1e-5 );
    EXPECT_NEAR( 6.0, Struct.getItem("z").number(), 1e-5 );
    EXPECT_FALSE( Struct.getItem("xy").isDefined() );
    EXPECT_EQ( "abcd", Struct.getItem("y").value<dictVariableValue>().getItem("l").string() );
  } catch ( ... )
  {
    FAIL() << "Exception!";
  }
}

// =========================================================


