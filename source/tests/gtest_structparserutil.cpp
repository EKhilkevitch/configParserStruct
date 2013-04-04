
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>

#include "configParserStruct/structparserutil.h"

using namespace configParserStruct;

// =========================================================

TEST( structParserInputString, set )
{
  structParserInputString String;
  String.set( "abc" );
  ASSERT_EQ( "abc", String.get() );
}

// ---------------------------------------------------------

TEST( structParserInputString, takeNextPart )
{
  structParserInputString String;
  String.set( "1234567890" );

  EXPECT_EQ( "123",   String.takeNextPart(3) );
  EXPECT_EQ( "45",    String.takeNextPart(2) );
  EXPECT_EQ( "",      String.takeNextPart(0) );
  EXPECT_EQ( "67890", String.takeNextPart(7) );
  EXPECT_EQ( "",      String.takeNextPart(3) );

  String.reset();
  EXPECT_EQ( String.get(), String.takeNextPart(10) );
  EXPECT_EQ( "", String.takeNextPart(3) );
}

// ---------------------------------------------------------

TEST( structParserInputString, putNextPartToBuffer )
{
  structParserInputString String;
  String.set( "1234567890" );

  char Buffer[ 2048 ];

  EXPECT_EQ( 3, String.putNextPartToBuffer(Buffer,3) );
  EXPECT_EQ( "123", std::string(Buffer) );
  
  EXPECT_EQ( 0,  String.putNextPartToBuffer(Buffer,0) );
  EXPECT_EQ( 7,  String.putNextPartToBuffer(Buffer,10) );
  EXPECT_EQ( "4567890", std::string(Buffer) );

}

// =========================================================


