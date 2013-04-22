
// =========================================================

#include <gtest/gtest.h>

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparserbuiltin.h"
#include "configParserStruct/structparserprogram.h"

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

TEST( variable, dictVariableValue_splitKey )
{
  EXPECT_EQ( "abc", dictVariableValue::splitKey("abc").first );
  EXPECT_EQ( "", dictVariableValue::splitKey("abc").second );
  
  EXPECT_EQ( "abc", dictVariableValue::splitKey("abc.xyz").first );
  EXPECT_EQ( "xyz", dictVariableValue::splitKey("abc.xyz").second );
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

// ---------------------------------------------------------

TEST( variable, dictVariableValue_getItem )
{
  dictVariableValue Value;
  Value.addItem( "x", createVariable(3) );
  Value.addItem( "x.n", createVariable(4) );
  Value.addItem( "x.m", createVariable(5) );
  Value.addItem( "w.r.t", createVariable(6) );
  Value.addItem( "y", createVariable(7) );

  EXPECT_NEAR( 0, Value.getItem("z").number(), 1e-5 );
  EXPECT_NEAR( 7, Value.getItem("y").number(), 1e-5 );
  EXPECT_NEAR( 2, Value.getItem("x").number(), 1e-5 );
  EXPECT_NEAR( 4, Value.getItem("x.n").number(), 1e-5 );
  EXPECT_NEAR( 5, Value.getItem("x.m").number(), 1e-5 );
  EXPECT_NEAR( 0, Value.getItem("x.o").number(), 1e-5 );
  EXPECT_NEAR( 6, Value.getItem("w.r.t").number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( variable, variablesListStack_globalName )
{
  ASSERT_EQ( "xyz", variablesListStack::globalName("::xyz") );
  ASSERT_EQ( "", variablesListStack::globalName("xyz") );
}

// ---------------------------------------------------------

TEST( variable, variablesListStack )
{

  variablesListStack List;
  ASSERT_EQ( 1, List.size() );

  List.clear();
  ASSERT_EQ( 1, List.size() );

  List.set( "a", createVariable(1) );
  List.set( "b", createVariable(2) );

  EXPECT_EQ( 2, List.listOfNames().size() );
  EXPECT_NEAR( 1, List.get("a").number(), 1e-5 );
  EXPECT_NEAR( 2, List.get("b").number(), 1e-5 );
  EXPECT_NEAR( 1, List.getFromTopOfStack("a").number(), 1e-5 );

  List.pushNewList();
  ASSERT_EQ( 2, List.size() );
  EXPECT_NEAR( 1, List.get("a").number(), 1e-5 );
  EXPECT_NEAR( 2, List.get("b").number(), 1e-5 );
  EXPECT_NEAR( 0, List.getFromTopOfStack("a").number(), 1e-5 );

  List.set( "a", createVariable(3) );
  EXPECT_NEAR( 3, List.get("a").number(),  1e-5 );
  EXPECT_NEAR( 1, List.get("::a").number(), 1e-5 );
  EXPECT_NEAR( 2, List.get("b").number(),  1e-5 );
  EXPECT_NEAR( 3, List.getFromTopOfStack("a").number(), 1e-5 );
  
  List.set( "::a", createVariable(4) );
  EXPECT_NEAR( 3, List.get("a").number(),  1e-5 );
  EXPECT_NEAR( 4, List.get("::a").number(), 1e-5 );
  EXPECT_NEAR( 3, List.getFromTopOfStack("a").number(), 1e-5 );

  List.popList();
  ASSERT_EQ( 1, List.size() );
  EXPECT_NEAR( 4, List.get("a").number(), 1e-5 );
  EXPECT_NEAR( 2, List.get("b").number(), 1e-5 );
 
  List.popList();
  ASSERT_EQ( 1, List.size() );
}

// ---------------------------------------------------------

TEST( variable, builtIn )
{
  program Program;

  variable Result;
  
  Result = piBuiltIn().execute( Program );
  EXPECT_NEAR( M_PI, Result.number(), 1e-5 );
}

// ---------------------------------------------------------

TEST( variable, referenceVariableValue_simple )
{
  program Program;
  Program.setNamedVariable( "a", createVariable(1) );
  Program.setNamedVariable( "b", createVariable<std::string>("xxx") );

  EXPECT_EQ( 1, referenceVariableValue("a").getValue(Program).integer() );
  EXPECT_EQ( "xxx", referenceVariableValue("b").getValue(Program).string() );
  EXPECT_FALSE( referenceVariableValue("c").getValue(Program).isDefined() );

  referenceVariableValue("a").setValue( &Program, createVariable(2) );
  EXPECT_EQ( 2, referenceVariableValue("a").getValue(Program).integer() );
  
  referenceVariableValue("a").setValue( &Program, createVariable<std::string>("nml") );
  EXPECT_EQ( "nml", referenceVariableValue("a").getValue(Program).string() );
  
  referenceVariableValue("c").setValue( &Program, createVariable(13) );
  EXPECT_EQ( 13, referenceVariableValue("c").getValue(Program).integer() );
}

// ---------------------------------------------------------

TEST( variable, referenceVariableValue_dict )
{
  program Program;

  dictVariableValue Dict;
  Dict.addItem( "key1", createVariable(1) );
  Dict.addItem( "key2", createVariable(2) );
  dictVariableValue ItemDict;
  ItemDict.addItem( "k5", createVariable(10) );
  ItemDict.addItem( "k6", createVariable(20) );
  Dict.addItem( "key3", ItemDict );
  Program.setNamedVariable( "Dict", Dict );

  EXPECT_EQ( 3, referenceVariableValue("Dict").getValue(Program).integer() );
  
  referenceVariableValue KeyRef("Dict");
  KeyRef.pushAttribute( createVariable<std::string>("key3") );
  EXPECT_EQ( 2, KeyRef.getValue(Program).integer() );
  
  KeyRef.pushAttribute( createVariable<std::string>("k6") );
  EXPECT_EQ( 20, KeyRef.getValue(Program).integer() );

  Program.setNamedVariable( "d", Dict );
  EXPECT_EQ( 3, referenceVariableValue("d").getValue(Program).integer() );
  referenceVariableValue("d").setValue( &Program, createVariable(13) );
  EXPECT_EQ( 13, referenceVariableValue("d").getValue(Program).integer() );

  KeyRef.setValue( &Program, createVariable(30) );
  EXPECT_EQ( 30, KeyRef.getValue(Program).integer() );

  KeyRef.popAttribute();
  EXPECT_EQ( 2, KeyRef.getValue(Program).integer() );
  KeyRef.setValue( &Program, createVariable(40) );
  EXPECT_EQ( 40, KeyRef.getValue(Program).integer() );
}

// =========================================================


