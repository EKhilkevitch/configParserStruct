
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "configParserStruct/structparservars.h"

using namespace configParserStruct;
using namespace structParserUtil;

// =========================================================

TEST( variable, variable )
{
  variable Var;

  EXPECT_FALSE( Var.isDefined() );
  EXPECT_FALSE( Var.boolean() );
  EXPECT_EQ( 0, Var.number() );
  EXPECT_EQ( "", Var.string() );
}

// ---------------------------------------------------------

TEST( variable, dictVariableValue_addItem )
{
  dictVariableValue Value;
  EXPECT_EQ( 0, Value.numberOfItems() );
  EXPECT_FALSE( Value.getItem("x").isDefined() );

  Value.addItem( "x", createVariable(3) );
  EXPECT_EQ( 1, Value.numberOfItems() );
  EXPECT_NEAR( 3, Value.getItem("x").number(), 1e-5 );

  Value.addItem( "y", createVariable(std::string("abc")) );
  EXPECT_EQ( 2, Value.numberOfItems() );
  EXPECT_NEAR( 3, Value.getItem("x").number(), 1e-5 );
  EXPECT_EQ( "abc", Value.getItem("y").string() );
  EXPECT_EQ( "{ .x = 3, .y = abc }", Value.string() );

  Value.addItem( "z.n", createVariable(4) );
  EXPECT_EQ( 3, Value.numberOfItems() );
  EXPECT_NEAR( 3, Value.getItem("x").number(), 1e-5 );
  EXPECT_EQ( "abc", Value.getItem("y").string() );
  EXPECT_EQ( "{ .n = 4 }", Value.getItem("z").string() );
  EXPECT_EQ( "{ .x = 3, .y = abc, .z = { .n = 4 } }", Value.string() );
  
  Value.addItem( "z.l", createVariable(5) );
  EXPECT_EQ( "{ .x = 3, .y = abc, .z = { .l = 5, .n = 4 } }", Value.string() );
  
  Value.addItem( "w.r.t", createVariable(6) );
  EXPECT_EQ( "{ .w = { .r = { .t = 6 } }, .x = 3, .y = abc, .z = { .l = 5, .n = 4 } }", Value.string() );
}

// =========================================================


