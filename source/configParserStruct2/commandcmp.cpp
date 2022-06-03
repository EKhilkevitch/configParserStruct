

// =====================================================

#include "configParserStruct2/commandcmp.h"
#include "configParserStruct2/memory.h"
#include "configParserStruct2/variablevalue.h"

#include <cassert>

// =====================================================
      
void configParserStruct::numEqCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  int CmpResult = 0;
  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
    CmpResult = ( Arg1.integer() == Arg2.integer() ) ? 1 : 0;
  else
    CmpResult = ( Arg1.real() == Arg2.real() ) ? 1 : 0;

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::numEqCommand::toString() const
{
  return "CMP EQ";
}

// -----------------------------------------------------

configParserStruct::numEqCommand* configParserStruct::numEqCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new numEqCommand();
  else
    return new ( Memory ) numEqCommand();
}

// =====================================================
      
void configParserStruct::numNeCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  int CmpResult = 0;
  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
    CmpResult = ( Arg1.integer() != Arg2.integer() ) ? 1 : 0;
  else
    CmpResult = ( Arg1.real() != Arg2.real() ) ? 1 : 0;

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::numNeCommand::toString() const
{
  return "CMP NE";
}

// -----------------------------------------------------

configParserStruct::numNeCommand* configParserStruct::numNeCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new numNeCommand();
  else
    return new ( Memory ) numNeCommand();
}

// =====================================================

void configParserStruct::numLtCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  int CmpResult = 0;
  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
    CmpResult = ( Arg2.integer() < Arg1.integer() ) ? 1 : 0;
  else
    CmpResult = ( Arg2.real() < Arg1.real() ) ? 1 : 0;

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::numLtCommand::toString() const
{
  return "CMP LT";
}

// -----------------------------------------------------

configParserStruct::numLtCommand* configParserStruct::numLtCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new numLtCommand();
  else
    return new ( Memory ) numLtCommand();
}

// =====================================================

void configParserStruct::numLeCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  int CmpResult = 0;
  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
    CmpResult = ( Arg2.integer() <= Arg1.integer() ) ? 1 : 0;
  else
    CmpResult = ( Arg2.real() <= Arg1.real() ) ? 1 : 0;

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::numLeCommand::toString() const
{
  return "CMP LE";
}

// -----------------------------------------------------

configParserStruct::numLeCommand* configParserStruct::numLeCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new numLeCommand();
  else
    return new ( Memory ) numLeCommand();
}

// =====================================================

void configParserStruct::numGtCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  int CmpResult = 0;
  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
    CmpResult = ( Arg2.integer() > Arg1.integer() ) ? 1 : 0;
  else
    CmpResult = ( Arg2.real() > Arg1.real() ) ? 1 : 0;

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::numGtCommand::toString() const
{
  return "CMP GT";
}

// -----------------------------------------------------

configParserStruct::numGtCommand* configParserStruct::numGtCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new numGtCommand();
  else
    return new ( Memory ) numGtCommand();
}

// =====================================================

void configParserStruct::numGeCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  int CmpResult = 0;
  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
    CmpResult = ( Arg2.integer() >= Arg1.integer() ) ? 1 : 0;
  else
    CmpResult = ( Arg2.real() >= Arg1.real() ) ? 1 : 0;

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::numGeCommand::toString() const
{
  return "CMP GE";
}

// -----------------------------------------------------

configParserStruct::numGeCommand* configParserStruct::numGeCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new numGeCommand();
  else
    return new ( Memory ) numGeCommand();
}

// =====================================================
      
void configParserStruct::strEqCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();
  const int CmpResult = ( Arg1.string() == Arg2.string() );

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::strEqCommand::toString() const
{
  return "CMP STREQ";
}

// -----------------------------------------------------

configParserStruct::strEqCommand* configParserStruct::strEqCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new strEqCommand();
  else
    return new ( Memory ) strEqCommand();
}

// =====================================================
      
void configParserStruct::strNeCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();
  const int CmpResult = ( Arg1.string() != Arg2.string() );

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::strNeCommand::toString() const
{
  return "CMP STRNE";
}

// -----------------------------------------------------

configParserStruct::strNeCommand* configParserStruct::strNeCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new strNeCommand();
  else
    return new ( Memory ) strNeCommand();
}

// =====================================================

void configParserStruct::strLtCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();
  const int CmpResult = ( Arg2.string() < Arg1.string() );

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::strLtCommand::toString() const
{
  return "CMP STRLT";
}

// -----------------------------------------------------

configParserStruct::strLtCommand* configParserStruct::strLtCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new strLtCommand();
  else
    return new ( Memory ) strLtCommand();
}

// =====================================================

void configParserStruct::strLeCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();
  const int CmpResult = ( Arg2.string() <= Arg1.string() );

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::strLeCommand::toString() const
{
  return "CMP STRLE";
}

// -----------------------------------------------------

configParserStruct::strLeCommand* configParserStruct::strLeCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new strLeCommand();
  else
    return new ( Memory ) strLeCommand();
}

// =====================================================

void configParserStruct::strGtCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();
  const int CmpResult = ( Arg2.string() > Arg1.string() );

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::strGtCommand::toString() const
{
  return "CMP STRGT";
}

// -----------------------------------------------------

configParserStruct::strGtCommand* configParserStruct::strGtCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new strGtCommand();
  else
    return new ( Memory ) strGtCommand();
}

// =====================================================

void configParserStruct::strGeCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();
  const int CmpResult = ( Arg2.string() >= Arg1.string() );

  Memory->pushToStack( variable(CmpResult) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::strGeCommand::toString() const
{
  return "CMP STRGE";
}

// -----------------------------------------------------

configParserStruct::strGeCommand* configParserStruct::strGeCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new strGeCommand();
  else
    return new ( Memory ) strGeCommand();
}

// =====================================================

