
// =========================================================

#include "configParserStruct/variable.h"

#include <gtest/gtest.h>

using namespace configParserStruct;

// =========================================================

TEST( variable, undef )
{
  variable Variable;

  EXPECT_EQ( 0, Variable.integer() );
  EXPECT_EQ( 0.0, Variable.real() );
  EXPECT_EQ( std::string(), Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, integer )
{
  variable Variable(5);

  EXPECT_EQ( 5, Variable.integer() );
  EXPECT_EQ( 5.0, Variable.real() );
  EXPECT_EQ( "5", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, real )
{
  variable Variable(5.5);

  EXPECT_EQ( 5,   Variable.integer() );
  EXPECT_EQ( 5.5, Variable.real() );
  EXPECT_EQ( "5.5", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, constructCopy )
{
  variable Src(5);
  variable Dst(Src);

  EXPECT_EQ( 5, Dst.integer() );
  EXPECT_EQ( 5.0, Dst.real() );
  EXPECT_EQ( "5", Dst.string() );
}

// ---------------------------------------------------------

TEST( variable, copy )
{
  variable Src(5);
  variable Dst(3);

  Dst = Src;

  EXPECT_EQ( 5, Dst.integer() );
  EXPECT_EQ( 5.0, Dst.real() );
  EXPECT_EQ( "5", Dst.string() );
}

// =========================================================

