
// =========================================================

#include <gtest/gtest.h>
#include <cstdio>

#include "configParserStruct/stringcast.h"

using namespace configParserStruct;

// =========================================================

TEST( stringCast, convertToString )
{
  ASSERT_EQ( "abc", convertToString("abc") );
  ASSERT_EQ( "5", convertToString(5) );
  ASSERT_EQ( "3.2", convertToString(3.2) );
}

// ---------------------------------------------------------

TEST( stringCast, convertFromString )
{
  ASSERT_EQ( "abc", convertFromString<std::string>("abc") );
  ASSERT_EQ( 5, convertFromString<int>(" 5 ") );
  ASSERT_NEAR( 3.2, convertFromString<double>("3.2"), 1e-5 );
}

// =========================================================


