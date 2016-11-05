
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <cmath>
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
  //EXPECT_EQ( 0, Program.variableNames().size() );

  Program.execute();
  EXPECT_EQ( 0, Program.stackSize() );
  //EXPECT_EQ( 0, Program.variableNames().size() );
}

// ---------------------------------------------------------

TEST( program, number )
{
  program Program;
  Program.build( "1;" );

  EXPECT_TRUE( Program.numberOfCommands() > 0 );
 
  Program.execute();
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( -1, Program.errorLine() );
 // EXPECT_EQ( 1, Program.variableNames().size() );
}

// ---------------------------------------------------------

TEST( program, assign )
{
  program Program;
  Program.build( "x = 1;" );

  //std::cout << Program.toString() << std::endl;

  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_TRUE( Program.numberOfCommands() > 0 );
 
  Program.execute();
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 1, Program.getNamedVariable("x").integer() );


  Program.rebuildAndExecute( "y = 3 + 2;" );
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 0, Program.getNamedVariable("x").integer() );
  EXPECT_EQ( 5, Program.getNamedVariable("y").integer() );
}

// ---------------------------------------------------------

TEST( program, clear )
{
  program Program;
  Program.build( "x = 1;" );
  Program.execute();
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );
 // EXPECT_EQ( 2, Program.variableNames().size() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );

  Program.clear();
  EXPECT_EQ( 0, Program.numberOfCommands() );
  EXPECT_EQ( 0, Program.stackSize() );
 // EXPECT_EQ( 0, Program.variableNames().size() );
}

// ---------------------------------------------------------

TEST( program, numericVariable )
{
  program Program;
  bool OK = Program.build( "x = 1;" );
  ASSERT_TRUE( OK );
  Program.execute();
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );

  OK = Program.rebuildAndExecute( "x = y + 1;" );
  ASSERT_TRUE( OK );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 0, Program.getNamedVariable("y").number(), 1e-5 );
  
  OK = Program.rebuildAndExecute( "x = 3.3; y = 4e2; z = 0.01;\n" );
  ASSERT_TRUE( OK );
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
 // EXPECT_EQ( 3, Program.variableNames().size() );
  EXPECT_EQ( "abc", Program.getNamedVariable("x").string() );
  EXPECT_EQ( "zz",  Program.getNamedVariable("y").string() );
}

// ---------------------------------------------------------

TEST( program, stringVariableSpecialSymbols )
{
  program Program;
  bool OK = Program.build( "x = 'a\\nbc'; y = \"z\\t\\065\\xf7z\";" );
  Program.execute();
  
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );
  
  EXPECT_EQ( 0, Program.stackSize() );
 // EXPECT_EQ( 3, Program.variableNames().size() );
  EXPECT_EQ( "a\nbc", Program.getNamedVariable("x").string() );
  EXPECT_EQ( "z\t\065\xf7z",  Program.getNamedVariable("y").string() );
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
  EXPECT_NEAR( 0, Program.getNamedVariable("y").number(), 1e-5 );
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

TEST( program, mod )
{
  program Program;
  bool OK = Program.rebuildAndExecute( "x = 8 % 3; y = 3.2 % 2; z = 4 % 7;" );
  
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 2.0, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 1.0, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( 4.0, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, addStr )
{
  program Program;
  bool OK = Program.rebuildAndExecute( 
    "x = 'ab' .+. 'cd';\n"
    "y = x .+. 13;\n"
    "z = 1 .+. 2;\n"
    "w = 'xy'; w .+=. \"abc\"; ");
  
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( "abcd",   Program.getNamedVariable("x").string() );
  EXPECT_EQ( "abcd13", Program.getNamedVariable("y").string() );
  EXPECT_EQ( "12",     Program.getNamedVariable("z").string() );
  EXPECT_EQ( "xyabc",  Program.getNamedVariable("w").string() );
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
  Program.rebuildAndExecute( "x = 2+1*3; y=2*(3+1); z = 11+(2+4)*(-7.0/2-4)-3;" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 5.0, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 8.0, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( -37, Program.getNamedVariable("z").number(), 1e-5 );
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

TEST( program, array )
{
  program Program;
  Program.rebuildAndExecute( "a = 3; b1 = [ 4, 2+1, a-1 ];\n" );

  ASSERT_EQ( -1, Program.errorLine() );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 3.0, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_EQ( "[ 4, 3, 2 ]", Program.getNamedVariable("b1").string() );
  EXPECT_EQ( 3, Program.getNamedVariable("b1").integer() );

  try
  {
    arrayVariableValue Array = Program.getNamedVariable("b1").value<arrayVariableValue>();
    EXPECT_EQ( 3, Array.numberOfItems() );
    EXPECT_EQ( 4, Array.getItem(0).integer() );
    EXPECT_EQ( 3, Array.getItem(1).integer() );
    EXPECT_EQ( 2, Array.getItem(2).integer() );
    EXPECT_EQ( 0, Array.getItem(9).integer() );
  } catch ( ... ) {
    FAIL() << "Exception!";
  }
  
  EXPECT_EQ( 4, Program.getNamedVariable("b1[0]").integer() );
  EXPECT_EQ( 3, Program.getNamedVariable("b1[1]").integer() );
  EXPECT_EQ( 2, Program.getNamedVariable("b1[2]").integer() );
}

// ---------------------------------------------------------

TEST( program, arrayFieldUse )
{
  program Program;
  Program.rebuildAndExecute( "b1 = [ 4, 3, 2 ];\n"
    "b2 = b1;\nc = b1[0]; d = b2[1+2/2]; e = b1[3]; b2[1] = 10; b3[4] = 2;" );
  
  //std::cout << Program.toString();
  
  ASSERT_EQ( -1, Program.errorLine() );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( "[ 4, 3, 2 ]", Program.getNamedVariable("b1").string() );
  EXPECT_EQ( "[ 4, 10, 2 ]", Program.getNamedVariable("b2").string() );
  EXPECT_EQ( "[ , , , , 2 ]", Program.getNamedVariable("b3").string() );
  EXPECT_EQ( 4, Program.getNamedVariable("c").integer() );
  EXPECT_EQ( 2, Program.getNamedVariable("d").integer() );
  EXPECT_FALSE( Program.getNamedVariable("e").isDefined() );
}

// ---------------------------------------------------------

TEST( program, arrayFieldDict )
{
  program Program;
  Program.build( "b1 = [ 4, 3, 2, { .x = 3, .y = 'abcd' } ];\n" );
  
  //std::cout << Program.toString();

  Program.execute();
  
  ASSERT_EQ( -1, Program.errorLine() );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( "[ 4, 3, 2, { .x = 3, .y = abcd } ]", Program.getNamedVariable("b1").string() );
}

// ---------------------------------------------------------

TEST( program, arrayFieldArray )
{
  program Program;
  Program.build( "b1 = [ 4, 3, 2, [ 7, 8 ] ];\n" );
  
  //std::cout << Program.toString();

  Program.execute();
  
  ASSERT_EQ( -1, Program.errorLine() );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( "[ 4, 3, 2, [ 7, 8 ] ]", Program.getNamedVariable("b1").string() );
}

// ---------------------------------------------------------

TEST( program, dict )
{
  program Program;
  Program.build( "a = 3; b = { .x = 0.1, .z = 2*a, .y = { .l = 'abcd' } };\n"
    "c = b; d = b.y;\n"
    "e = { .a = 2 }; e['m'] = 12; f['n'] = 13; f['sp ace' .+. '!!!'] = 'ab';\n"
    "g.x = 12; g.z = 14; g.b = 'xyz';\n");
  //std::cerr << Program.toString() << std::endl;
  Program.execute();
  
  ASSERT_EQ( -1, Program.errorLine() );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 3.0, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_EQ( "{ .x = 0.1, .y = { .l = abcd }, .z = 6 }", Program.getNamedVariable("b").string() );
  EXPECT_EQ( Program.getNamedVariable("b").string(), Program.getNamedVariable("c").string() );
  EXPECT_EQ( "{ .l = abcd }", Program.getNamedVariable("d").string() );
  EXPECT_EQ( "{ .a = 2, .m = 12 }", Program.getNamedVariable("e").string() );
  EXPECT_EQ( "{ .n = 13, .sp ace!!! = ab }", Program.getNamedVariable("f").string() );
  try
  {
    dictVariableValue Struct = Program.getNamedVariable("b").value<dictVariableValue>();
    EXPECT_EQ( 3, Struct.numberOfItems() );
    EXPECT_NEAR( 0.1, Struct.getItem("x").number(), 1e-5 );
    EXPECT_NEAR( 6.0, Struct.getItem("z").number(), 1e-5 );
    EXPECT_FALSE( Struct.getItem("xy").isDefined() );
    EXPECT_EQ( "abcd", Struct.getItem("y").value<dictVariableValue>().getItem("l").string() );
  } catch ( ... ) {
    FAIL() << "Exception!";
  }
  
  EXPECT_EQ( "{ .b = xyz, .x = 12, .z = 14 }", Program.getNamedVariable("g").string() );
}

// ---------------------------------------------------------

TEST( program, dictFieldAssign )
{
  program Program;
  Program.build( "a = { .x = 1, .y = 2 }; a.x = 3; b.z = 4; c = a.x + 2;" );
  Program.execute();
 
  //std::cerr << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( "{ .x = 3, .y = 2 }", Program.getNamedVariable("a").string() );
  EXPECT_EQ( "{ .z = 4 }", Program.getNamedVariable("b").string() );
  EXPECT_EQ( "5", Program.getNamedVariable("c").string() );
}

// ---------------------------------------------------------

TEST( program, dictLastComma )
{
  program Program;
  Program.build( "a = { .x = 1, .y = 2, .z = 3, };" );
  Program.execute();
 
  //std::cerr << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( "{ .x = 1, .y = 2, .z = 3 }", Program.getNamedVariable("a").string() );
  EXPECT_EQ( "3", Program.getNamedVariable("a.z").string() );
}

// ---------------------------------------------------------

TEST( program, dictErrorComma )
{
  program Program;
  Program.build( "a = { .x = 1, .y = 2 .z = 3, };" ); // No comma between y and z
  Program.execute();
  
  ASSERT_EQ( 0, Program.errorLine() );
}

// ---------------------------------------------------------

TEST( program, dictFieldArray )
{
  program Program;
  Program.build( "a = { .x = 1, .y = [ 2, 3 ] };" );
  Program.execute();
 
  //std::cerr << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( "{ .x = 1, .y = [ 2, 3 ] }", Program.getNamedVariable("a").string() );
  EXPECT_EQ( "[ 2, 3 ]", Program.getNamedVariable("a.y").string() );
}

// ---------------------------------------------------------

TEST( program, comments )
{
  program Program;
  Program.rebuildAndExecute( "x = 1;\n# y = 2;\n\n z = { .a = 2, # *3\n .b = 4 # aaa, \n #.d = 7 \n };\n" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 1.0, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 0.0, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_EQ( "{ .a = 2, .b = 4 }", Program.getNamedVariable("z").string() );
}

// ---------------------------------------------------------

TEST( program, emptyExpression )
{
  program Program;
  bool OK;
  OK = Program.rebuildAndExecute( "x = exp(2);;\n"); 

  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
}

// ---------------------------------------------------------

TEST( program, getLastExpressionReuslt )
{
  program Program;
  Program.rebuildAndExecute( "x = 2; 3+4;" );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 7.0, Program.getLastExpressionReuslt().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, onExecuteVariableNames )
{
  program Program;
  Program.rebuildAndExecute( "x = 2; 3+4; yyy=4; z.l = 2;" );

  std::set<std::string> Variables = Program.onExecuteVariableNames();

  EXPECT_EQ( 1, Variables.count("x") );
  EXPECT_EQ( 0, Variables.count("a") );
  EXPECT_EQ( 1, Variables.count("yyy") );
  EXPECT_EQ( 0, Variables.count("z") );
  EXPECT_EQ( 1, Variables.count("z.l") );
}

// ---------------------------------------------------------

TEST( program, incorrectProgram )
{
  program Program;
  bool OK = Program.rebuildAndExecute( "x = 2;\n y=x+1;\n z = 3+;\nn=3-;\n" );

  EXPECT_FALSE( OK );
  EXPECT_EQ( 2, Program.errorLine() );

  OK = Program.rebuildAndExecute( "x = 2;\n y=x+1;\n z = 3+1; \n" );
  EXPECT_TRUE( OK );
  EXPECT_EQ( -1, Program.errorLine() );

  OK = Program.rebuildAndExecute( "x = 2;y=x+1;z = 3;" );
  EXPECT_TRUE( OK );
  EXPECT_EQ( -1, Program.errorLine() );

  OK = Program.rebuildAndExecute( "\nx = 2\ny=x+1\nz = 3\n" );
  EXPECT_FALSE( OK );
  EXPECT_EQ( 2, Program.errorLine() );
  
  OK = Program.rebuildAndExecute( "x = 2;y=x+1;z = 3;" );
  EXPECT_TRUE( OK );
  EXPECT_EQ( -1, Program.errorLine() );
  
  OK = Program.rebuildAndExecute( "\nx = { .a = 1,\n .b = 3\n .c = 4 };\n" );
  EXPECT_FALSE( OK );
  EXPECT_EQ( 3, Program.errorLine() );
  
  OK = Program.rebuildAndExecute( "\nx = { .a = 1,\n .b = { .f1 = 3 }\n .c = { .f2 = 4 } };\n" );
  EXPECT_FALSE( OK );
  EXPECT_EQ( 3, Program.errorLine() );
  
  OK = Program.rebuildAndExecute( "x = 2;y=x+1;z = 3;" );
  EXPECT_TRUE( OK );
  EXPECT_EQ( -1, Program.errorLine() );
}

// ---------------------------------------------------------

TEST( program, newLine )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "!NL-\nx = 2;y=x+1;z = 3;" );
  EXPECT_TRUE( OK );

  OK = Program.rebuildAndExecute( "!NL+\nx = 2\ny=x+1\nz = 3\n" );
  EXPECT_TRUE( OK );
  
  OK = Program.rebuildAndExecute( "\nx = 2\ny=x+1\nz = 3\n" );
  EXPECT_FALSE( OK );
}

// ---------------------------------------------------------

TEST( program, functionDeclaration )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "f1 = func { x = 1; };" );
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_TRUE( Program.getNamedVariable("f1").isDefined() );
  
//  std::cout << Program.toString() << std::endl;
}

// ---------------------------------------------------------

TEST( program, functionCall )
{
  program Program;
  bool OK;

  OK = Program.build( "f1 = func { x = 1; return x+2; }; y = f1(); z = 2*f1() + 3; a = 1;" );
 // std::cout << Program.toString() << std::endl;
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );

  Program.execute();

 // std::cout << Program.toString() << std::endl;

  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_TRUE( Program.getNamedVariable("f1").isDefined() );
  EXPECT_FALSE( Program.getNamedVariable("x").isDefined() );
  EXPECT_NEAR( 3, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( 9, Program.getNamedVariable("z").number(), 1e-5 );
  EXPECT_NEAR( 1, Program.getNamedVariable("a").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionCallWithArgs )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "f1 = func { x = $1; return 2*x + 3*$2 + 4*$3; }; z = f1(4,5,6) + 0.5; " );
  
  //std::cout << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );

  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_TRUE( Program.getNamedVariable("f1").isDefined() );
  EXPECT_NEAR( 2*4 + 3*5 + 4*6 + 0.5, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionComplex )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "f1 = func { return $1*2; }; f2 = func { return f1($1) + 3.1; }; f1(2.1) + 0.1*f2(4,5);" );
  
  //std::cout << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_TRUE( Program.getNamedVariable("f1").isDefined() );
  EXPECT_NEAR( 2.1*2 + 0.1*(2*4+3.1), Program.getLastExpressionReuslt().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionVars )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "x = 1; f1 = func { x = 2; z = 3; }; y1 = x; f1(); y2 = x;" );

  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 2, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 1, Program.getNamedVariable("y1").number(), 1e-5 );
  EXPECT_NEAR( 2, Program.getNamedVariable("y2").number(), 1e-5 );
  EXPECT_NEAR( 0, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionLocalVars )
{
  program Program;
  bool OK;

  OK = Program.build( "x = 1; y = 2; f1 = func { @@x = 2; @@y = 3; y += 1; z = 3; }; a = x; f1(); b = x;" );
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );

  //std::cerr << Program.toString() << std::endl;

  Program.execute();
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 2, Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( 1, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_NEAR( 1, Program.getNamedVariable("b").number(), 1e-5 );
  EXPECT_NEAR( 0, Program.getNamedVariable("z").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionComplexFrams )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "x=0; f1 = func { x=1; return 2; }; f2 = func { x = 30; return f1() + 5 + x; }; a=f2();" );
  
  //std::cout << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_TRUE( Program.getNamedVariable("f1").isDefined() );
  EXPECT_NEAR( 1, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 2 + 5 + 1, Program.getNamedVariable("a").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionComplesFib )
{
  program Program;
  bool OK;

  OK = Program.build( "fib = func { @@x = $1; "
    "if ( x <= 0 ) { return 0; }"
    "if ( x <= 1 ) { return 1; }"
    "@@y1 = fib(x-1); @@y2=fib(x-2); return y1+y2; }; "
    "a = fib(1); b = fib(2); c=fib(10);" );
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
 
  //std::cout << Program.toString() << std::endl;
  
  Program.execute();
  ASSERT_EQ( 0, Program.stackSize() );
  ASSERT_EQ( -1, Program.errorLine() );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_TRUE( Program.getNamedVariable("fib").isDefined() );
  EXPECT_NEAR( 1, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_NEAR( 1, Program.getNamedVariable("b").number(), 1e-5 );
  EXPECT_NEAR( 55, Program.getNamedVariable("c").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionReturnContaners )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "f1 = func { return { .a = 3, .b = $1, .c = $2*2 }; };\n"
    "f2 = func { return [ 1, $1*2, $2*3 ]; };\n"
    "x = f1(3,4); y = f2(5,6);\n"
    );
  
 // std::cout << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_TRUE( Program.getNamedVariable("f1").isDefined() );
  EXPECT_TRUE( Program.getNamedVariable("f2").isDefined() );
  EXPECT_EQ( "{ .a = 3, .b = 3, .c = 8 }", Program.getNamedVariable("x").string() );
  EXPECT_EQ( "[ 1, 10, 18 ]", Program.getNamedVariable("y").string() );
}

// ---------------------------------------------------------

TEST( program, functionUnknown )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "x = f1(3); f2 = func { 1+2; }; y = f2(3)+2;" );
  
 // std::cout << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_FALSE( Program.getNamedVariable("f1").isDefined() );
  EXPECT_NEAR( 0, Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( 2, Program.getNamedVariable("y").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( program, functionBuiltIn )
{
  program Program;
  bool OK;

  OK = Program.rebuildAndExecute( "x = exp(2); y = sin(0.3) + cos(4); z = pi();\n"
    "#println(1+3,' = 1 + 3');\n"
    "#println('>>',x,'<<');\n"
    "a = 2; z1 = exp( a+pi() );\n"
    "z2 = pow( 3.0, 5.5 );\n"
    "z3 = atan2( 4, 7 );\n"
    "z4 = abs(-3.4); z5 = defined(z4); z6 = defined(ABC);\n"
    "z7 = erf(0.5); \n"
    "z8 = erfc(0.5);\n"
    "z9 = erf(-0.3) + 0.7*erfc(-0.4); \n"
    "z10 = sqr(-3);\n"
    "z11 = max( 3, 4 );\n"
    "z12 = min( 3, 4 );\n"
    );
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );
  ASSERT_TRUE( OK );
 
//  std::cout << Program.toString() << std::endl;
  
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_NEAR( std::exp(2), Program.getNamedVariable("x").number(), 1e-5 );
  EXPECT_NEAR( std::sin(0.3) + std::cos(4), Program.getNamedVariable("y").number(), 1e-5 );
  EXPECT_NEAR( M_PI, Program.getNamedVariable("z").number(), 1e-5 );
  EXPECT_NEAR( 2, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_NEAR( std::exp( 2 + M_PI ), Program.getNamedVariable("z1").number(), 1e-5 );
  EXPECT_NEAR( std::pow( 3, 5.5 ), Program.getNamedVariable("z2").number(), 1e-5 );
  EXPECT_NEAR( std::atan2( 4, 7 ), Program.getNamedVariable("z3").number(), 1e-5 );
  EXPECT_NEAR( 3.4, Program.getNamedVariable("z4").number(), 1e-5 );
  EXPECT_TRUE( Program.getNamedVariable("z5").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("z6").boolean() );
  EXPECT_NEAR( 0.52049987781, Program.getNamedVariable("z7").number(), 1e-7 );
  EXPECT_NEAR( 0.47950012218, Program.getNamedVariable("z8").number(), 1e-7 );
  EXPECT_NEAR( 0.67124788907, Program.getNamedVariable("z9").number(), 1e-7 );
  EXPECT_NEAR( 9, Program.getNamedVariable("z10").number(), 1e-7 );
  EXPECT_NEAR( 4, Program.getNamedVariable("z11").number(), 1e-7 );
  EXPECT_NEAR( 3, Program.getNamedVariable("z12").number(), 1e-7 );
}

// ---------------------------------------------------------

TEST( program, cmpNumbers )
{
  program Program;
  Program.rebuildAndExecute( "a = 1>2;b = 1<2;c = 1==1; d = 1!=1; e = 1==2; f = 1 >= 2; g = 1 <= 2; h = 2 <= 1;\n"
    "i = 1+2 > 2.5; j = 1-2 < -1; \n"
    "x = 0; y = 1;\n");
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_FALSE( Program.getNamedVariable("a").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("b").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("c").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("d").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("e").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("f").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("g").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("h").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("i").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("j").boolean() );
  
  EXPECT_FALSE( Program.getNamedVariable("x").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("y").boolean() );
}

// ---------------------------------------------------------

TEST( program, cmpStrings )
{
  program Program;
  Program.rebuildAndExecute( "a = 'a'.>.'b'; b = 'a'.<.'z';\n"
    "c = 'abc'.==.'abc'; d = 'abcd'.!=.'abcd'; e = 'gfez'.==.'gfe';\n"
    "f = 'zxy' .>=. 'yyy'; g = 'abc' .<=. 'cde'; h = 'zxy' .<=. 'abc';\n"
    "i = ( 'b' .+. 'cd' ) .>. 'baaa'; j = 'abcd' .>. 'abc'; \n"
    "x = ''; y = 'a';\n");
  
//  std::cout << Program.toString() << std::endl;
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_FALSE( Program.getNamedVariable("a").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("b").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("c").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("d").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("e").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("f").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("g").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("h").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("i").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("j").boolean() );
  
  EXPECT_FALSE( Program.getNamedVariable("x").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("y").boolean() );
}

// ---------------------------------------------------------

TEST( program, booleanOp )
{
  program Program;
  Program.rebuildAndExecute( "a = 1 && 2;\nb = 0 && 1;\n c = 1 || 0;\n d = 0 || 0; \n e = ! 1; \n f = !!0; \n g = !0;\n" );
  
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );
  
  EXPECT_TRUE( Program.getNamedVariable("a").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("b").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("c").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("d").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("e").boolean() );
  EXPECT_FALSE( Program.getNamedVariable("f").boolean() );
  EXPECT_TRUE( Program.getNamedVariable("g").boolean() );
}

// ---------------------------------------------------------

TEST( program, ternary )
{
  program Program;
  bool OK = Program.rebuildAndExecute( "a = 2 > 1 ? 3 : 4;\n"
    "b = ( 2 < 1 ) ? 5 : 6;\n"
    "sign = func { return ( $1 > 0 ) ? +1 : ( $1 < 0 ) ? -1 : 0; };\n"
    "x = sign(-3); y = sign(+2); z = sign(0);" );

  ASSERT_TRUE( OK );
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );
  
  EXPECT_EQ( 3, Program.getNamedVariable("a").integer() );
  EXPECT_EQ( 6, Program.getNamedVariable("b").integer() );
  
  EXPECT_EQ( -1, Program.getNamedVariable("x").integer() );
  EXPECT_EQ( +1, Program.getNamedVariable("y").integer() );
  EXPECT_EQ(  0, Program.getNamedVariable("z").integer() );
}

// ---------------------------------------------------------

TEST( program, varsAsBool )
{
  program Program;
  bool OK = Program.rebuildAndExecute( "x1 = 0 ? 3 : 4;\n"
    "x2 = 2 ? 3 : 4;\n"
    "y1 = a ? 3 : 4;\n"
    "a = 1; y2 = a ? 3 : 4;\n"
    "z1 = '' ? 3 : 4;\n"
    "z2 = '1' ? 3 : 4;\n"
    "z3 = 'abc' ? 3 : 4;\n");

  ASSERT_TRUE( OK );
  ASSERT_EQ( -1, Program.errorLine() );
  EXPECT_EQ( 0, Program.stackSize() );
  
  EXPECT_EQ( 4, Program.getNamedVariable("x1").integer() );
  EXPECT_EQ( 3, Program.getNamedVariable("x2").integer() );

  EXPECT_EQ( 4, Program.getNamedVariable("y1").integer() );
  EXPECT_EQ( 3, Program.getNamedVariable("y2").integer() );
  
  EXPECT_EQ( 4, Program.getNamedVariable("z1").integer() );
  EXPECT_EQ( 3, Program.getNamedVariable("z2").integer() );
  EXPECT_EQ( 3, Program.getNamedVariable("z3").integer() );

}

// ---------------------------------------------------------

TEST( program, DISABLED_factorial )
{
  program Program;
  bool OK = Program.rebuildAndExecute( "fact = func { return ($1 >= 1) ? fact($1-1)*$1 : 1; };\n"
    "fact(8);\n" );

  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( 40320, Program.getLastExpressionReuslt().integer() );
}

// ---------------------------------------------------------

TEST( program, ifelse )
{
  program Program;
  bool OK = Program.rebuildAndExecute( "if ( 1 > 2 ) { x = 1; }\n"
    " if ( 2 > 1 ) { y = 2; } else { y = 3; }\n"
    " if ( 1 < 2 ) { z = 4; } else { z = 5*2; }\n");

  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( 0, Program.getNamedVariable("x").integer() );
  EXPECT_EQ( 2, Program.getNamedVariable("y").integer() );
  EXPECT_EQ( 4, Program.getNamedVariable("z").integer() );
}

// ---------------------------------------------------------

TEST( program, elif )
{
  program Program;
  bool OK = Program.build( "if ( 1 > 2 ) { x = 1; } elif ( 3 > 2 ) { x = 2; } else { x = 3; }\n"
    " if ( 2 > 1 ) { y = 2; } elif ( 3 > 2 ) { y = 3; }\n"
    " if ( 3 < 2 ) { z = 4; } elif ( 4 < 3 ) { z = 5; }\n");

//  std::cout << Program.toString() << std::endl;

  Program.execute();

  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( 2, Program.getNamedVariable("x").integer() );
  EXPECT_EQ( 2, Program.getNamedVariable("y").integer() );
  EXPECT_EQ( 0, Program.getNamedVariable("z").integer() );
}

// ---------------------------------------------------------

TEST( program, whileCycle )
{
  program Program;
  bool OK = Program.build( "while (0) { x = 1; }\n"
    "y = 2;\n"
    "i = z = 1; while (i<10) { z *= i; i += 1; }\n" );
  //std::cout << Program.toString() << std::endl;
  Program.execute();

  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( 0, Program.getNamedVariable("x").integer() );
  EXPECT_EQ( 2, Program.getNamedVariable("y").integer() );
  EXPECT_EQ( 10, Program.getNamedVariable("i").integer() );
  EXPECT_EQ( 362880, Program.getNamedVariable("z").integer() );
}

// ---------------------------------------------------------

TEST( program, symbolicRef )
{
  program Program;
  bool OK = Program.build( "a = 3; b =4; bc = 5; s1 = 'b';\n"
    "${'a'} = 10; ${s1} = 20; ${s1.+.'c'}=30; ${'d'} = 40;\n" );
  //std::cout << Program.toString() << std::endl;
  Program.execute();

  ASSERT_EQ( -1, Program.errorLine() );
  ASSERT_TRUE( OK );
  EXPECT_EQ( 0, Program.stackSize() );

  EXPECT_EQ( 10, Program.getNamedVariable("a").integer() );
  EXPECT_EQ( 20, Program.getNamedVariable("b").integer() );
  EXPECT_EQ( 30, Program.getNamedVariable("bc").integer() );
  EXPECT_EQ( 40, Program.getNamedVariable("d").integer() );
}

// =========================================================


