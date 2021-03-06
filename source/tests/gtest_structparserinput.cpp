
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>

#include "configParserStruct/structparserinput.h"

using namespace configParserStruct;
using namespace structParserUtil;

// =========================================================

TEST( inputString, set )
{
  inputString String;
  String.set( "abc" );
  ASSERT_EQ( "abc", String.get() );
}

// ---------------------------------------------------------

TEST( inputString, takeNextPart )
{
  inputString String;
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

TEST( inputString, putNextPartToBuffer )
{
  inputString String;
  String.set( "1234567890" );

  char Buffer[ 2048 ];
  std::memset( Buffer, '\0', sizeof(Buffer) );

  EXPECT_EQ( 3, String.putNextPartToBuffer(Buffer,3) );
  EXPECT_EQ( "123", std::string(Buffer) );
  
  EXPECT_EQ( 0,  String.putNextPartToBuffer(Buffer,0) );
  EXPECT_EQ( 7,  String.putNextPartToBuffer(Buffer,10) );
  EXPECT_EQ( "4567890", std::string(Buffer) );
}

// ---------------------------------------------------------

TEST( inputString, putNextPartToBuffer_2 )
{
  inputString String;
  String.set( "1234567890" );

  char Buffer[ 2048 ];
  
  std::memset( Buffer, '\0', sizeof(Buffer) );
  Buffer[ 3 ] = 'Z';
  
  EXPECT_EQ( 3, String.putNextPartToBuffer(Buffer,3) );
  EXPECT_EQ( "123Z", std::string(Buffer) );
}

// =========================================================


