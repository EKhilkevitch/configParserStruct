
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercommand.h"

using namespace configParserStruct;
using namespace structParserUtil;

// =========================================================

TEST( commandsList, commandsList )
{
  commandsList List;
  
  EXPECT_EQ( 0, List.size() );
  EXPECT_EQ( 0, List.currentCommandIndex() );
  EXPECT_EQ( std::string()+typeid( nopCommand ).name(), std::string()+List.currentCommand().actionType().name() );
  EXPECT_TRUE( List.isAllCommandsDone() );

  List.push( pushValueCommand( createVariable(3) ) );
  List.push( pushValueCommand( createVariable(5) ) );

  EXPECT_EQ( 2, List.size() );
  EXPECT_EQ( 0, List.currentCommandIndex() );
  EXPECT_EQ( std::string()+typeid( pushValueCommand ).name(), std::string()+List.currentCommand().actionType().name() );
  EXPECT_FALSE( List.isAllCommandsDone() );
}

// ---------------------------------------------------------

TEST( commandsList, execute )
{
  commandsList List;
  
  List.push( pushValueCommand( createVariable(3) ) );
  List.push( nopCommand() );
  List.push( pushValueCommand( createVariable(5) ) );
  List.push( pushValueCommand( createVariable(7) ) );
  List.push( addCommand() );
  List.push( addCommand() );

  program Program;
  List.execute( &Program );

  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 15, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, pushValueCommand )
{
  program Program;

  pushValueCommand Command( createVariable(3) );
  Command.execute(&Program);

  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 3, Program.topStackVariable().number(), 1e-5 );
  
  pushValueCommand( createVariable(0.0) ).execute(&Program);
  EXPECT_EQ( 2, Program.stackSize() );
  EXPECT_NEAR( 0, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, pushVariableCommand )
{
  program Program;
  Program.setNamedVariable( "a", createVariable(4) );

  pushVariableCommand( "a" ).execute( &Program );

  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 4, Program.topStackVariable().number(), 1e-5 );
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

  pushValueCommand( createVariable(3) ).execute( &Program );
  pushValueCommand( createVariable(4) ).execute( &Program );
  ASSERT_EQ( 2, Program.stackSize() );
  EXPECT_NEAR( 4, Program.topStackVariable().number(), 1e-5 );

  popCommand Command;
  Command.execute( &Program );
  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 3, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, addCommand )
{
  program Program;

  pushValueCommand( createVariable(3) ).execute( &Program );
  pushValueCommand( createVariable(4) ).execute( &Program );
  addCommand().execute( &Program );

  ASSERT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 7, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, subCommand )
{
  program Program;

  pushValueCommand( createVariable(3) ).execute( &Program );
  pushValueCommand( createVariable(4) ).execute( &Program );
  subCommand().execute( &Program );

  ASSERT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( -1, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, assignCommand )
{
  program Program;

  pushValueCommand( createVariable(3) ).execute( &Program );
  assignCommand( "a" ).execute( &Program );

  ASSERT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 3, Program.topStackVariable().number(), 1e-5 );
  EXPECT_NEAR( 3, Program.getNamedVariable("a").number(), 1e-5 );
}

// =========================================================


