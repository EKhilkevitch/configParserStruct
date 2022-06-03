
// =====================================================

#include "configParserStruct2/commandbinops.h"
#include "configParserStruct2/memory.h"
#include "configParserStruct2/variablevalue.h"
#include "configParserStruct2/reference.h"

#include <limits>
#include <cmath>
#include <cassert>

// =====================================================
      
void configParserStruct::addCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
  {
    const variable Result( Arg1.integer() + Arg2.integer() );
    Memory->pushToStack( Result );
  } else {
    const variable Result( Arg1.real() + Arg2.real() );
    Memory->pushToStack( Result );
  }

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::addCommand::toString() const
{
  return "ADD";
}

// -----------------------------------------------------

configParserStruct::addCommand* configParserStruct::addCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new addCommand();
  else
    return new ( Memory ) addCommand();
}

// =====================================================
      
void configParserStruct::subCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
  {
    const variable Result( Arg2.integer() - Arg1.integer() );
    Memory->pushToStack( Result );
  } else {
    const variable Result( Arg2.real() - Arg1.real() );
    Memory->pushToStack( Result );
  }

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::subCommand::toString() const
{
  return "SUB";
}

// -----------------------------------------------------

configParserStruct::subCommand* configParserStruct::subCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new subCommand();
  else
    return new ( Memory ) subCommand();
}

// =====================================================

void configParserStruct::mulCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName )
  {
    const variable Result( Arg1.integer() * Arg2.integer() );
    Memory->pushToStack( Result );
  } else {
    const variable Result( Arg1.real() * Arg2.real() );
    Memory->pushToStack( Result );
  }

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::mulCommand::toString() const
{
  return "MUL";
}

// -----------------------------------------------------

configParserStruct::mulCommand* configParserStruct::mulCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new mulCommand();
  else
    return new ( Memory ) mulCommand();
}

// =====================================================

void configParserStruct::divCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName && Arg1.integer() != 0 )
  {
    const variable Result( Arg2.integer() / Arg1.integer() );
    Memory->pushToStack( Result );
  } else {
    const variable Result( Arg2.real() / Arg1.real() );
    Memory->pushToStack( Result );
  }

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::divCommand::toString() const
{
  return "DIV";
}

// -----------------------------------------------------

configParserStruct::divCommand* configParserStruct::divCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new divCommand();
  else
    return new ( Memory ) divCommand();
}

// =====================================================

void configParserStruct::modCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  if ( Arg1.type() == integerVariableValue::TypeName && Arg2.type() == integerVariableValue::TypeName && Arg1.integer() != 0 )
  {
    const variable Result( Arg2.integer() % Arg1.integer() );
    Memory->pushToStack( Result );
  } else {
    const variable Result( std::fmod( Arg2.real(), Arg1.real() ) );
    Memory->pushToStack( Result );
  }

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::modCommand::toString() const
{
  return "MOD";
}

// -----------------------------------------------------

configParserStruct::modCommand* configParserStruct::modCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new modCommand();
  else
    return new ( Memory ) modCommand();
}

// =====================================================

void configParserStruct::boolAndCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  const variable Result( Arg1.boolean() && Arg2.boolean() ? 1 : 0 );
  Memory->pushToStack( Result );

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::boolAndCommand::toString() const
{
  return "AND BOOL";
}

// -----------------------------------------------------

configParserStruct::boolAndCommand* configParserStruct::boolAndCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new boolAndCommand();
  else
    return new ( Memory ) boolAndCommand();
}

// =====================================================

void configParserStruct::boolOrCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  const variable Result( Arg1.boolean() || Arg2.boolean() ? 1 : 0 );
  Memory->pushToStack( Result );

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::boolOrCommand::toString() const
{
  return "OR BOOL";
}

// -----------------------------------------------------

configParserStruct::boolOrCommand* configParserStruct::boolOrCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new boolOrCommand();
  else
    return new ( Memory ) boolOrCommand();
}

// =====================================================

void configParserStruct::boolNotCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg = Memory->popFromStack();

  const variable Result( Arg.boolean() ? 0 : 1 );
  Memory->pushToStack( Result );

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::boolNotCommand::toString() const
{
  return "NOT BOOL";
}

// -----------------------------------------------------

configParserStruct::boolNotCommand* configParserStruct::boolNotCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new boolNotCommand();
  else
    return new ( Memory ) boolNotCommand();
}

// =====================================================
      
void configParserStruct::strAddCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg1 = Memory->popFromStack();
  const variable Arg2 = Memory->popFromStack();

  const variable Result( Arg2.string() + Arg1.string() );
  Memory->pushToStack( Result );

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::strAddCommand::toString() const
{
  return "ADD STR";
}

// -----------------------------------------------------

configParserStruct::strAddCommand* configParserStruct::strAddCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new strAddCommand();
  else
    return new ( Memory ) strAddCommand();
}

// =====================================================

void configParserStruct::joinArrayCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Arg = Memory->popFromStack();
  variable *Top = Memory->topStackValue();

  assert( Top != NULL );
  
  Top->setByRef( reference( std::numeric_limits<size_t>::max(), reference::ArrayIndex ), Arg );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::joinArrayCommand::toString() const
{
  return "JOIN TO ARR";
}

// -----------------------------------------------------

configParserStruct::joinArrayCommand* configParserStruct::joinArrayCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new joinArrayCommand();
  else
    return new ( Memory ) joinArrayCommand();
}

// =====================================================

configParserStruct::joinDictCommand::joinDictCommand( const char *Name ) :
  command( variable( reference( Name, reference::DictKey ) ) )
{
}

// -----------------------------------------------------

configParserStruct::joinDictCommand::joinDictCommand( const variable &Variable ) :
  command( Variable )
{
}

// -----------------------------------------------------

void configParserStruct::joinDictCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Value = Memory->popFromStack();
  variable *Top = Memory->topStackValue();

  assert( Top != NULL );
  
  Top->setByRef( argument().ref(), Value );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::joinDictCommand::toString() const
{
  return "JOIN TO DICT";
}

// -----------------------------------------------------

configParserStruct::joinDictCommand* configParserStruct::joinDictCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new joinDictCommand(argument());
  else
    return new ( Memory ) joinDictCommand(argument());
}

// =====================================================

