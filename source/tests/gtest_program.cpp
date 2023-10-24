
// =========================================================

#include "configParserStruct/program.h"
#include "configParserStruct/exception.h"
#include "configParserStruct/command.h"

#include <iostream>
#include <cmath>

#include <gtest/gtest.h>

using namespace configParserStruct;

// =========================================================

TEST( program, empty )
{
  program Program;
  Program.build( ";" );

  ASSERT_EQ( 1, Program.programText().size() );
  EXPECT_EQ( "HALT", Program.programText()[0].toString() );
}

// ---------------------------------------------------------

TEST( program, lastResult_1 )
{
  program Program;
  Program.build( "3;" );
  Program.run();

  EXPECT_EQ( 3, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, lastResult_2 )
{
  program Program;
  Program.build( "3.7;" );
  Program.run();

  EXPECT_EQ( 3.7, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, lastResult_3 )
{
  program Program;
  Program.build( "'abc def';" );
  Program.run();

  EXPECT_EQ( "abc def", Program.programMemory().lastResult().string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, add_1 )
{
  program Program;
  Program.build( "3 + 4;" );
  Program.run();

  EXPECT_EQ( 7, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, wrong )
{
  program Program;
  try
  {
    Program.build( "3 + ;" );
    FAIL() << "No exception!";
  } catch ( const exception& ) { }
}

// ---------------------------------------------------------

TEST( program, add_2 )
{
  program Program;
  Program.build( "3 + 4.5;" );
  Program.run();

  EXPECT_EQ( 7.5, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, add_3 )
{
  program Program;
  Program.build( "'3' + 4.5;" );
  Program.run();

  EXPECT_EQ( 7.5, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, sub_1 )
{
  program Program;
  Program.build( "3 - 4.5;" );
  Program.run();

  EXPECT_EQ( -1.5, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, unminus_1 )
{
  program Program;
  Program.build( "-4.5;" );
  Program.run();

  EXPECT_EQ( -4.5, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, unminus_2 )
{
  program Program;
  Program.build( "- ( -4.5 );" );
  Program.run();

  EXPECT_EQ( 4.5, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, unminus_3 )
{
  program Program;
  Program.build( "-( 1 + 2 );" );
  Program.run();

  EXPECT_EQ( -3, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, add_mul_1 )
{
  program Program;
  Program.build( "2 * ( 3 + 4.5 );" );
  Program.run();

  EXPECT_EQ( 15.0, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, add_mul_2 )
{
  program Program;
  Program.build( "( 3 + 4.5 ) * 2;" );
  Program.run();

  EXPECT_EQ( 15.0, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, add_mul_3 )
{
  program Program;
  Program.build( "1 + 3 * 2;" );
  Program.run();

  EXPECT_EQ( 7, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, div_1 )
{
  program Program;
  Program.build( "10 / 2;" );
  Program.run();

  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, div_2 )
{
  program Program;
  Program.build( "10 / 2.5;" );
  Program.run();

  EXPECT_EQ( 4, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, div_3 )
{
  program Program;
  Program.build( "10 / 0;" );
  Program.run();

  EXPECT_EQ( 10/0.0, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, mod_1 )
{
  program Program;
  Program.build( "17 % 7;" );
  Program.run();

  EXPECT_EQ( 3, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, mod_2 )
{
  program Program;
  Program.build( "17 % 7.5;" );
  Program.run();

  EXPECT_EQ( 2, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_eq_1 )
{
  program Program;
  Program.build( "1 + 4 == 2 + 3;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_eq_2 )
{
  program Program;
  Program.build( "1 + 4.5 == 2 + 6;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_streq_1 )
{
  program Program;
  Program.build( " \"xxx\" .==. \"xxx\";" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_streq_2 )
{
  program Program;
  Program.build( " \"xxx\" .==. \"yyy\";" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_strneq_1 )
{
  program Program;
  Program.build( " \"xxx\" .!=. \"yyy\";" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_ne_1 )
{
  program Program;
  Program.build( "1 != 2;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_ne_2 )
{
  program Program;
  Program.build( "2 != 2;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_lt_1 )
{
  program Program;
  Program.build( "1 < 2;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_lt_2 )
{
  program Program;
  Program.build( "3 < 2;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_le_1 )
{
  program Program;
  Program.build( "1 <= 2;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_le_2 )
{
  program Program;
  Program.build( "2 <= 2;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_le_3 )
{
  program Program;
  Program.build( "3 <= 2;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}


// ---------------------------------------------------------

TEST( program, cmp_gt_1 )
{
  program Program;
  Program.build( "1 > 2;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_gt_2 )
{
  program Program;
  Program.build( "3 > 2;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_ge_1 )
{
  program Program;
  Program.build( "1 >= 2;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_ge_2 )
{
  program Program;
  Program.build( "2 >= 2;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, cmp_ge_3 )
{
  program Program;
  Program.build( "3 >= 2;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, booland_1 )
{
  program Program;
  Program.build( "2 && 1;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, booland_2 )
{
  program Program;
  Program.build( "0 && 1;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, boolor_1 )
{
  program Program;
  Program.build( "2 || 0;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, boolor_2 )
{
  program Program;
  Program.build( "0 || 0;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, boolnot_1 )
{
  program Program;
  Program.build( " ! 4;" );
  Program.run();

  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, boolnot_2 )
{
  program Program;
  Program.build( " ! 0;" );
  Program.run();

  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, stradd_1 )
{
  program Program;
  Program.build( " 'abc' .+. 'xy' ;" );
  Program.run();

  EXPECT_EQ( "abcxy", Program.programMemory().lastResult().string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, stradd_2 )
{
  program Program;
  Program.build( " 'abc' .+. 3.5;" );
  Program.run();

  EXPECT_EQ( "abc3.5", Program.programMemory().lastResult().string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, assign_1 )
{
  program Program;
  Program.build( "x = 3;" );
  Program.run();

  EXPECT_EQ( 3, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 3, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, assign_2 )
{
  program Program;
  Program.build( "x = 3; x = 5 + 4.5;" );
  Program.run();

  EXPECT_EQ( 9.5, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 9.5, Program.programMemory().findValueByName("x",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, assign_3 )
{
  program Program;
  Program.build( "a = 'a\nb'; b = 'a\t'; c =  \"str\"; d = 'a\\nb';" );
  Program.run();

  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("a",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("b",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("c",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("d",named::GlobalScope) );
  
  EXPECT_EQ( "a\nb", Program.programMemory().findValueByName("a",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( "a\t",  Program.programMemory().findValueByName("b",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( "str",  Program.programMemory().findValueByName("c",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( "a\nb", Program.programMemory().findValueByName("d",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, assign_add_1 )
{
  program Program;
  Program.build( "x = 3; x += 2;" );
  Program.run();

  EXPECT_EQ( 5, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, assign_deref_1 )
{
  program Program;
  Program.build( "x = 3; 4 + x;" );
  Program.run();

  EXPECT_EQ( 7, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 3, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, assign_deref_2 )
{
  program Program;
  Program.build( "x = 3; y = 4 + x; zzz = x + y;" );
  Program.run();

  EXPECT_EQ( 10, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ(  3, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  EXPECT_EQ(  7, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("zzz",named::GlobalScope) );
  EXPECT_EQ( 10, Program.programMemory().findValueByName("zzz",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, comments )
{
  program Program;
  Program.build( "x = 3; # x = 4; y = 1;\nz = 5;" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL == Program.programMemory().findValueByName("y",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  
  EXPECT_EQ(  3, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ(  5, Program.programMemory().findValueByName("z",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, if_1 )
{
  program Program;
  Program.build( "if ( 1 + 1 == 2 ) { x = 5; }" );
  Program.run();
  
  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, if_2 )
{
  program Program;
  Program.build( "if ( 1 + 1 == 2 ) { x = 5; } x = x + 1;" );
  Program.run();
  
  EXPECT_EQ( 6, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 6, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, if_3 )
{
  program Program;
  Program.build( "if ( 1 + 1 == 4 ) { x = 5; } x = x + 1;" );
  Program.run();
  
  EXPECT_EQ( 1, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 1, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, ifelse_1 )
{
  program Program;
  Program.build( "if ( 1 + 3 == 4 ) { x = 5; } else { x = 6; }" );
  Program.run();
  
  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, ifelse_2 )
{
  program Program;
  Program.build( "if ( 1 + 3 == 5 ) { x = 5; } else { x = 6; }" );
  Program.run();
  
  EXPECT_EQ( 6, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 6, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, elif_1 )
{
  program Program;
  Program.build( "x = 3; if ( x == 5 ) { z = 5; } elif ( x == 4 ) { z = 6; } elif ( x == 3 ) { z = 7; } elif ( x == 2 ) { z = 8; }" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("z",named::GlobalScope) );
  EXPECT_EQ( 7, Program.programMemory().findValueByName("z",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, while_1 )
{
  program Program;
  Program.build( "x = 0; while ( x < 5 ) { x += 2; }" );
  Program.run();

  EXPECT_EQ( 6, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 6, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, thrdop_1 )
{
  program Program;
  Program.build( "x = ( 1 + 2 == 3 ) ? 5 : 6;" );
  Program.run();
  
  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, thrdop_2 )
{
  program Program;
  Program.build( "x = ( 1 + 2 == 7 ) ? 5 : 6;" );
  Program.run();
  
  EXPECT_EQ( 6, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 6, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, thrdop_3 )
{
  program Program;

  try
  {
    Program.build( "t < 0 ? " );
    FAIL() << "No exception";
  } catch ( const exception& ) {}
}

// ---------------------------------------------------------

TEST( program, func_1 )
{
  program Program;
  Program.build( "x = 2; f = func { @x = @x + 1; }; f();" );
  Program.run();
  
  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 3, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_2 )
{
  program Program;
  Program.build( "f = func { return 1 + 3; }; x = f();" );
  Program.run();
  
  EXPECT_EQ( 4, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 4, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_3 )
{
  program Program;
  Program.build( "f = func { return ( $1 + $2 ) * 100; }; x = f(2,5);" );
  Program.run();
  
  EXPECT_EQ( 700, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 700, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_4 )
{
  program Program;
  Program.build( "f = func { return $0 * 100; }; x = f(2,5,3.1);" );
  Program.run();
  
  EXPECT_EQ( 300, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 300, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_local_1 )
{
  program Program;
  Program.build( "x = 3; y = 4; f = func { x = 4; @y = 5; }; f();" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 3, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 5, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_5 )
{
  program Program;
  Program.build( "g = func { return $1 + 1; }; f = func { return g($1) * 5; }; f(3);" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("g",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 5*(3+1), Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_6 )
{
  program Program;
  Program.build( "f = func { if ( $1 < 5 ) { return 1; } return $1 + 1; }; x = f(3); y = f(10);" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ(  1, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 11, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_7 )
{
  program Program;
  Program.build( "g = func { return $1 + 2; }; f = func { if ( $1 < 5 ) { return 1; } else { return g($1+1); } }; x = f(3); y = f(10);" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ(  1, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 13, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_8 )
{
  program Program;
  Program.build( "g = func { x = $1; return x + 2; }; f = func { x = $1; @x += 1; return g(x+3); }; x = 98; y = f(10);" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 99, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 15, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_9 )
{
  program Program;
  Program.build( "sqr = func { return $1 * $1; }; f = func { return sqr($1) + sqr( sqr($2) ); }; y = f(3,4);" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  EXPECT_EQ( 3*3 + 4*4*4*4, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 3*3 + 4*4*4*4, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString() << Program.programText().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_wrong_1 )
{
  program Program;
  Program.build( "x = $3;" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 0, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_wrong_2 )
{
  program Program;
  Program.build( "x = 98; y = f(10);" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  ASSERT_TRUE( NULL == Program.programMemory().findValueByName("f",named::GlobalScope) );
  EXPECT_EQ( 98, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ(  0, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_wrong_3 )
{
  program Program;
  Program.build( "x = 42; y = x();" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("y",named::GlobalScope) );
  EXPECT_EQ( 42, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ(  0, Program.programMemory().findValueByName("y",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_wrong_4 )
{
  program Program;
  Program.build( "f = func { return $1*1.5 + $2 + $3; }; f(2);" );
  Program.run();
  
  EXPECT_EQ( 3.0, Program.programMemory().lastResult().real() ) << Program.programMemory().toDebugString() << Program.programText().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_wrong_5 )
{
  program Program;
  Program.build( "x = 3; return 5;" );
  Program.run();
  
  EXPECT_EQ( 3, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString() << Program.programText().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_fact_1 )
{
  program Program;
  Program.build( "fact = func { if ( $1 <= 1 ) { return 1; } else { return $1 * fact($1-1); } }; fact(5);" );
  Program.run();
  
  EXPECT_EQ( 120, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_fib_1 )
{
  program Program;
  Program.build( "fib = func { if ( $1 <= 2 ) { return 1; } else { return fib($1-1) + fib($1-2); } }; fib(8);" );
  Program.run();
  
  EXPECT_EQ( 21, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_buildin_1 )
{
  program Program;
  Program.build( "S = sin(0.3); C = cos(0.7); E = exp( 1.4 ); Pi = pi();" );
  Program.run();

  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("S",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("C",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("E",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("Pi",named::GlobalScope) );
  EXPECT_EQ( std::sin(0.3), Program.programMemory().findValueByName("S",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( std::cos(0.7), Program.programMemory().findValueByName("C",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( std::exp(1.4), Program.programMemory().findValueByName("E",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 4*std::atan(1.0), Program.programMemory().findValueByName("Pi",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, func_buildin_2 )
{
  program Program;
  Program.build( " A = abs(-3); S = sqr(4); P = pow( 3, 4 ); M = max(3,4); println( \"println 3^4 = \", P );" );
  Program.run();

  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("A",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("S",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("P",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("M",named::GlobalScope) );
  EXPECT_EQ( std::fabs(-3), Program.programMemory().findValueByName("A",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 4.0*4.0, Program.programMemory().findValueByName("S",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( std::pow(3.0,4.0), Program.programMemory().findValueByName("P",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 4.0, Program.programMemory().findValueByName("M",named::GlobalScope)->real() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, array_1 )
{
  program Program;
  Program.build( "x = arr[2];" );
  Program.run();
  
  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) ) << Program.programMemory().toDebugString() << "\n" << Program.programText().toDebugString();
  EXPECT_EQ( 0, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, array_2 )
{
  program Program;
  Program.build( "arr[2] = 5; x = arr[2];" );
  Program.run();
  
  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString() << "\n" << Program.programText().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("arr",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, array_3 )
{
  program Program;
  Program.build( "arr[2][6] = 5; x = arr[2][6];" );
  Program.run();
  
  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("arr",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, array_4 )
{
  program Program;
  Program.build( "arr[2] = 5; arr[3] = 6; arr[0] = 9; arr[2] -= 1;" );
  Program.run();
  
  EXPECT_EQ( 4, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("arr",named::GlobalScope) );
  EXPECT_EQ( "[ 9, , 4, 6 ]", Program.programMemory().findValueByName("arr",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, array_5 )
{
  program Program;
  Program.build( "arr = [ 3, 5, 7, 9 ];" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("arr",named::GlobalScope) );
  EXPECT_EQ( "[ 3, 5, 7, 9 ]", Program.programMemory().findValueByName("arr",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, array_6 )
{
  program Program;
  Program.build( "arr = [ 3, 5, 7, 9 ]; arr[1] = arr[2] - 1; arr[4-1] = 9.5;" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("arr",named::GlobalScope) );
  EXPECT_EQ( "[ 3, 6, 7, 9.5 ]", Program.programMemory().findValueByName("arr",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, array_func_1 )
{
  program Program;
  Program.build( "f = func { return [ $1, $1*2, $1*3, $1*4 ]; } arr = f(3);" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("arr",named::GlobalScope) );
  EXPECT_EQ( "[ 3, 6, 9, 12 ]", Program.programMemory().findValueByName("arr",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_1 )
{
  program Program;
  Program.build( "x = dict{'a'};" );
  Program.run();
  
  EXPECT_EQ( 0, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  EXPECT_EQ( 0, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_2 )
{
  program Program;
  Program.build( "dict{'key'} = 5; x = dict{'key'};" );
  Program.run();
  
  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}


// ---------------------------------------------------------

TEST( program, dict_3 )
{
  program Program;
  Program.build( "dict{'key_1'}{'key_2'} = 5; x = dict{'key_1'}{'key_2'};" );
  Program.run();
  
  EXPECT_EQ( 5, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( 5, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_4 )
{
  program Program;
  Program.build( "dict{'a'} = 5; dict{'b'} = 6; dict{'c'} = 9; dict{'a'} -= 1;" );
  Program.run();
  
  EXPECT_EQ( 4, Program.programMemory().lastResult().integer() ) << Program.programMemory().toDebugString();
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( "{ .a = 4, .b = 6, .c = 9 }", Program.programMemory().findValueByName("dict",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_5 )
{
  program Program;
  Program.build( "dict = { .a = 4, .b = 6, .c = 7 };" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( "{ .a = 4, .b = 6, .c = 7 }", Program.programMemory().findValueByName("dict",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_6 )
{
  program Program;
  Program.build( "dict.a = 1; dict.b = 3+1;" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( "{ .a = 1, .b = 4 }", Program.programMemory().findValueByName("dict",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_7 )
{
  program Program;
  Program.build( "dict.a = 1; dict.b = dict.a + 2; dict.c = 4; dict.c += 5;" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( "{ .a = 1, .b = 3, .c = 9 }", Program.programMemory().findValueByName("dict",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_8 )
{
  program Program;
  Program.build( "dict = { .a = 4, .b = 6, .c = 7, };" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( "{ .a = 4, .b = 6, .c = 7 }", Program.programMemory().findValueByName("dict",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_9 )
{
  program Program;
  Program.build( "dict = {};" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( "{  }", Program.programMemory().findValueByName("dict",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, dict_10 )
{
  program Program;
  Program.build( "dict = { .a = 1, .b = [ 3, 4, 5 ] };" );
  Program.run();
  
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("dict",named::GlobalScope) );
  EXPECT_EQ( "{ .a = 1, .b = [ 3, 4, 5 ] }", Program.programMemory().findValueByName("dict",named::GlobalScope)->string() ) << Program.programMemory().toDebugString();
}

// ---------------------------------------------------------

TEST( program, variables_1 )
{
  program Program;
  Program.build( "dict.a = 1; dict.b = 3+1; x = 7; f = func { y = 4; x += 1; @z = 4; }" );

  std::set<std::string> Set = Program.programText().variables();

  EXPECT_EQ( 1, Set.count("dict") );
  EXPECT_EQ( 1, Set.count("x") );
  EXPECT_EQ( 1, Set.count("z") );
  EXPECT_EQ( 0, Set.count("y") );
  EXPECT_EQ( 0, Set.count("f") );
}

// ---------------------------------------------------------

TEST( program, setValueByName_1 )
{
  program Program;
  Program.build( "x = a; b = b + 1;" );
  Program.programMemory().setValueByName( "a", variable(3), named::PresetScope );
  Program.programMemory().setValueByName( "b", variable(5), named::PresetScope );
  Program.run();

  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("x",named::GlobalScope) );
  ASSERT_TRUE( NULL != Program.programMemory().findValueByName("b",named::GlobalScope) );
  EXPECT_EQ( 3, Program.programMemory().findValueByName("x",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString();
  EXPECT_EQ( 6, Program.programMemory().findValueByName("b",named::GlobalScope)->integer() ) << Program.programMemory().toDebugString() << Program.programText().toDebugString();
}

// ---------------------------------------------------------

TEST( program, copy )
{
  program Program;
  Program.build( "x = 1; struct.field = 3;" );

  const text Copy = Program.programText();

  /* Проверить что в копии все ссылки ведут на память в том же тексте, что и оригинал */
  
  //std::cout << Program.programText().toDebugString() << std::endl; 
  //std::cout << Copy.toDebugString() << std::endl; 

}

// =========================================================

