
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercommand.h"

using namespace configParserStruct;
using namespace structParser;

// =========================================================

TEST( commandsList, commandsList )
{
  commandsList List;
  
  EXPECT_EQ( 0, List.size() );
  EXPECT_EQ( 0, List.currentCommandIndex() );
  EXPECT_EQ( std::string()+typeid( nopCommand ).name(), std::string()+List.currentCommand().actionType().name() );
  EXPECT_TRUE( List.isAllCommandsDone() );

  List.push( pushCommand( createVariable(3) ) );
  List.push( pushCommand( createVariable(5) ) );

  EXPECT_EQ( 2, List.size() );
  EXPECT_EQ( 0, List.currentCommandIndex() );
  EXPECT_EQ( std::string()+typeid( pushCommand ).name(), std::string()+List.currentCommand().actionType().name() );
  EXPECT_FALSE( List.isAllCommandsDone() );
}

// ---------------------------------------------------------

TEST( command, pushCommand )
{
  program Program;

  pushCommand Command( createVariable(3) );
  Command.execute(&Program);

  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 3, Program.topVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, nopCommand )
{
  program Program;

  nopCommand Command;
  Command.execute(&Program);

  EXPECT_EQ( 0, Program.stackSize() );
}

// ---------------------------------------------------------

TEST( command, popCommand )
{
  program Program;

  pushCommand( createVariable(3) ).execute( &Program );
  pushCommand( createVariable(4) ).execute( &Program );
  ASSERT_EQ( 2, Program.stackSize() );
  EXPECT_NEAR( 4, Program.topVariable().number(), 1e-5 );

  popCommand Command;
  Command.execute( &Program );
  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 3, Program.topVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, addCommand )
{
  program Program;

  pushCommand( createVariable(3) ).execute( &Program );
  pushCommand( createVariable(4) ).execute( &Program );
  addCommand().execute( &Program );

  ASSERT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 7, Program.topVariable().number(), 1e-5 );
}

// =========================================================


