
// =========================================================

#include <gtest/gtest.h>

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercommand.h"
#include "configParserStruct/structparserbuiltin.h"

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

TEST( command, assignVariableCommand )
{
  program Program;
  EXPECT_NEAR( 0, Program.getLastExpressionReuslt().number(), 1e-5 );

  pushValueCommand( referenceVariableValue("a") ).execute( &Program );
  pushValueCommand( createVariable(3) ).execute( &Program );
  assignVariableCommand().execute( &Program );

  ASSERT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 3, Program.getNamedVariable("a").number(), 1e-5 );
  EXPECT_NEAR( 0, Program.getLastExpressionReuslt().number(), 1e-5 );

  assignLastExpressionCommand().execute( &Program );
  EXPECT_NEAR( 3, Program.getLastExpressionReuslt().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, setDictFieldCommand ) 
{
  program Program;

  pushValueCommand( dictVariableValue() ).execute( &Program );
  pushValueCommand( createVariable(5.5) ).execute( &Program );
  setDictFieldCommand( "field" ).execute( &Program );
  
  ASSERT_EQ( 1, Program.stackSize() );
  EXPECT_EQ( "{ .field = 5.5 }", Program.topStackVariable().string() );
  try
  {
    EXPECT_NEAR( 5.5, Program.topStackVariable().value<dictVariableValue>().getItem("field").number(), 1e-5 );
  } catch ( ... )
  {
    FAIL() << "Exception!";
  }
}

// ---------------------------------------------------------

TEST( command, jumpToCommand )
{
  program Program;
  Program.pushCommand( pushValueCommand( createVariable(3.3) ) );
  Program.pushCommand( assignVariableCommand() );
  Program.pushCommand( pushValueCommand( createVariable(4) ) );
  Program.pushCommand( pushValueCommand( createVariable(5) ) );
  EXPECT_EQ( 0, Program.currentCommandIndex() );

  jumpToCommand(2).execute( &Program );
  EXPECT_EQ( 1, Program.currentCommandIndex() );
  ASSERT_EQ( 0, Program.stackSize() );

  Program.clear();
  Program.pushCommand( pushValueCommand( createVariable(3.0) ) ); // 0
  Program.pushCommand( jumpToCommand(3) ); // 1
  Program.pushCommand( pushValueCommand( createVariable(6) ) ); // 2
  Program.pushCommand( pushValueCommand( createVariable(7) ) ); // 3
  Program.execute();

  ASSERT_EQ( 2, Program.stackSize() );
  EXPECT_EQ( 7, Program.topStackVariable().integer() );
}

// ---------------------------------------------------------

TEST( command, callCommand )
{
  program Program;
  for ( unsigned i = 0; i < 10U; i++ )
    Program.pushCommand( nopCommand() );

  Program.setNamedVariable( "func", commandAddressVariableValue(5) );
  jumpToCommand(2).execute( &Program );
  EXPECT_EQ( 1, Program.currentCommandIndex() );

  callCommand( "func" ).execute( &Program );
  EXPECT_EQ( 4, Program.currentCommandIndex() );
  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 2, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, callCommandBuiltIn )
{
  program Program;
  Program.setNamedVariable( "pi", makeBuiltIn<mathZeroArgumentBuiltIn>("pi",M_PI) );
  callCommand( "pi" ).execute( &Program );

  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( M_PI, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, retCommand )
{
  program Program;
  
  Program.pushCommand( nopCommand() ); // 0
  Program.pushCommand( nopCommand() ); // 1
  Program.pushCommand( pushValueCommand( createVariable(3.3) ) ); // 2
  Program.pushCommand( pushValueCommand( createVariable(1.0) ) ); // 3
  Program.pushCommand( addCommand() ); // 4
  Program.pushCommand( retCommand() ); // 5
  Program.pushCommand( pushValueCommand( createVariable(10) ) ); // 6
  Program.pushCommand( callCommand("func") ); // 7
  Program.pushCommand( pushValueCommand( createVariable(20) ) ); // 8
  Program.pushCommand( popCommand() );
  Program.setCurrentCommandIndex(7);
  Program.setNamedVariable( "func", commandAddressVariableValue(2) );

  EXPECT_EQ( 7, Program.currentCommandIndex() );

  Program.executeOneCommand();
  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 8, Program.topStackVariable().number(), 1e-5 );
  EXPECT_EQ( 2, Program.currentCommandIndex() );

  Program.executeOneCommand();
  EXPECT_EQ( 2, Program.stackSize() );
  EXPECT_NEAR( 3.3, Program.topStackVariable().number(), 1e-5 );
  
  Program.executeOneCommand();
  Program.executeOneCommand();
  EXPECT_EQ( 2, Program.stackSize() );
  EXPECT_NEAR( 4.3, Program.topStackVariable().number(), 1e-5 );

  Program.executeOneCommand();
  EXPECT_EQ( 8, Program.currentCommandIndex() );
  EXPECT_EQ( 1, Program.stackSize() );
  EXPECT_NEAR( 4.3, Program.topStackVariable().number(), 1e-5 );
  
  Program.executeOneCommand();
  EXPECT_EQ( 9, Program.currentCommandIndex() );
  EXPECT_EQ( 2, Program.stackSize() );
  EXPECT_NEAR( 20, Program.topStackVariable().number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( command, pushVarFrameCommand )
{
  program Program;
  EXPECT_EQ( 1, Program.functionLevel() );

  pushVarFrameCommand().execute( &Program );
  EXPECT_EQ( 2, Program.functionLevel() );

  popVarFrameCommand().execute( &Program );
  EXPECT_EQ( 1, Program.functionLevel() );
}

// ---------------------------------------------------------

TEST( command, pushArgumentCommand )
{
  program Program;
  pushVarFrameCommand().execute( &Program );
  EXPECT_EQ( 0, Program.getNamedVariable("$N").integer() );
  EXPECT_EQ( 0, Program.stackSize() );

  pushValueCommand( createVariable(3) ).execute( &Program );
  pushArgumentCommand().execute( &Program );
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 1, Program.getNamedVariable("$N").integer() );
  EXPECT_EQ( 3, Program.getNamedVariable("$1").integer() ); 
  
  pushValueCommand( createVariable(4) ).execute( &Program );
  pushArgumentCommand().execute( &Program );
  EXPECT_EQ( 0, Program.stackSize() );
  EXPECT_EQ( 2, Program.getNamedVariable("$N").integer() );
  EXPECT_EQ( 3, Program.getNamedVariable("$1").integer() ); 
  EXPECT_EQ( 4, Program.getNamedVariable("$2").integer() ); 
}

// ---------------------------------------------------------



// =========================================================


