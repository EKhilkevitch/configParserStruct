
// =========================================================

#include "configParserStruct2/lexinput.h"

#include <gtest/gtest.h>

// =========================================================

TEST( lexInput, lexInput_1 )
{
  CPSSPU_setParserInputString( "123456789" );

  char String[1024] = { '\0' };
  size_t Taked = 0;

  Taked = CPSSPU_takeNextFromParserInputString( String, 20 );
  EXPECT_EQ( 9, Taked );
  EXPECT_STREQ( "123456789", String );
  
  Taked = CPSSPU_takeNextFromParserInputString( String, 5 );
  EXPECT_EQ( 0, Taked );
  EXPECT_STREQ( "", String );
}

// ---------------------------------------------------------

TEST( lexInput, lexInput_2 )
{
  CPSSPU_setParserInputString( "123456789" );

  char String[1024] = { '\0' };
  size_t Taked = 0;

  Taked = CPSSPU_takeNextFromParserInputString( String, 3 );
  EXPECT_EQ( 2, Taked );
  EXPECT_STREQ( "12", String );
  
  Taked = CPSSPU_takeNextFromParserInputString( String, 5 );
  EXPECT_EQ( 4, Taked );
  EXPECT_STREQ( "3456", String );
  
  Taked = CPSSPU_takeNextFromParserInputString( String, 5 );
  EXPECT_EQ( 3, Taked );
  EXPECT_STREQ( "789", String );
  
  Taked = CPSSPU_takeNextFromParserInputString( String, 5 );
  EXPECT_EQ( 0, Taked );
  EXPECT_STREQ( "", String );
}

// ---------------------------------------------------------

TEST( lexInput, lexInput_3 )
{
  char String[1024] = { '\0' };
  size_t Taked = 0;

  CPSSPU_clearParserInputString();

  Taked = CPSSPU_takeNextFromParserInputString( String, 20 );
  EXPECT_EQ( 0, Taked );
  EXPECT_STREQ( "", String );
}

// =========================================================


