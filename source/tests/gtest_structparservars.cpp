
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

// =========================================================


