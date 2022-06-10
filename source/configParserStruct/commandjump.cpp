
// =====================================================

#include "configParserStruct/commandjump.h"
#include "configParserStruct/memory.h"
#include "configParserStruct/reference.h"
#include "configParserStruct/buildinfunc.h"

#include <sstream>
#include <cassert>

// =====================================================
      
void configParserStruct::nopCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

configParserStruct::nopCommand* configParserStruct::nopCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new nopCommand();
  else
    return new ( Memory ) nopCommand();
}

// -----------------------------------------------------
      
std::string configParserStruct::nopCommand::toString() const
{
  return "NOP";
}

// =====================================================
      
void configParserStruct::haltCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  Memory->setHaltFlag();
}

// -----------------------------------------------------

configParserStruct::haltCommand* configParserStruct::haltCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new haltCommand();
  else
    return new ( Memory ) haltCommand();
}

// -----------------------------------------------------
      
std::string configParserStruct::haltCommand::toString() const
{
  return "HALT";
}

// =====================================================

configParserStruct::jumpIfFalseCommand::jumpIfFalseCommand( size_t Index ) :
  command( variable( reference( Index, reference::InstructionPointer ) ) )
{
}

// -----------------------------------------------------

configParserStruct::jumpIfFalseCommand::jumpIfFalseCommand( const variable &Variable ) :
  command( Variable )
{
}

// -----------------------------------------------------

void configParserStruct::jumpIfFalseCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  
  const variable Value = Memory->popFromStack();
  if ( Value.boolean() )
  {
    Memory->jumpToNextCommand();
  } else {
    Memory->jumpToCommand( argument().ref().asInstructionPointer() );
  }
}

// -----------------------------------------------------

configParserStruct::jumpIfFalseCommand* configParserStruct::jumpIfFalseCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new jumpIfFalseCommand( argument() );
  else
    return new ( Memory ) jumpIfFalseCommand( argument() );
}

// -----------------------------------------------------
      
std::string configParserStruct::jumpIfFalseCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "JMP IF FALSE TO " << argument();
  return Stream.str();
}

// =====================================================

configParserStruct::jumpCommand::jumpCommand( size_t Index ) :
  command( variable( reference( Index, reference::InstructionPointer ) ) )
{
}

// -----------------------------------------------------

configParserStruct::jumpCommand::jumpCommand( const variable &Variable ) :
  command( Variable )
{
}

// -----------------------------------------------------

void configParserStruct::jumpCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  
  const variable Instruction = argument();
  Memory->jumpToCommand( Instruction.ref().asInstructionPointer() );
}

// -----------------------------------------------------

configParserStruct::jumpCommand* configParserStruct::jumpCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new jumpCommand( argument() );
  else
    return new ( Memory ) jumpCommand( argument() );
}

// -----------------------------------------------------
      
std::string configParserStruct::jumpCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "JMP TO " << argument();
  return Stream.str();
}

// =====================================================

configParserStruct::callCommand::callCommand( const variable &Argument ) :
  command( Argument )
{
}

// -----------------------------------------------------
      
configParserStruct::callCommand::callCommand( const char *Name ) :
  command( variable( reference( Name, reference::GlobalName ) ) )
{
}

// -----------------------------------------------------

void configParserStruct::callCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  
  const char *FunctionName = argument().ref().asGlobalName();
  const variable *Function = Memory->findValueByReference( FunctionName, named::GlobalScope );

  if ( Function == NULL )
  {
    const buildInFunction* BuildinFunction = Memory->findFunctionByReference( FunctionName );
  
    size_t PrevStackSize = Memory->baseStackPointer();
    Memory->setBaseStackPointer( Memory->stackSize() + 1 );
    const variable Result = ( BuildinFunction != NULL ) ? BuildinFunction->call( *Memory ) : variable();
    Memory->setBaseStackPointer( PrevStackSize );
  
    const size_t ArgsCount = Memory->topStackValue()->ref().asArgumentsCount();
    Memory->truncateStack( Memory->stackSize() - ArgsCount - 1 );
    variable *Top = Memory->topStackValue();
    *Top = Result;
    Memory->jumpToNextCommand();
    return;
  }

  const reference &FunctionReference = Function->ref();

  if ( ! FunctionReference.hasType(reference::InstructionPointer) )
  {
    const size_t ArgsCount = Memory->topStackValue()->ref().asArgumentsCount();
    Memory->truncateStack( Memory->stackSize() - ArgsCount - 1 );
    variable *Top = Memory->topStackValue();
    *Top = variable();
    Memory->jumpToNextCommand();
    return;
  }

  Memory->pushToStack( variable( reference( Memory->instructionPointer(), reference::InstructionPointer ) ) );
  Memory->setBaseStackPointer( Memory->stackSize() );
  Memory->pushLocalNamedFrame();
  Memory->jumpToCommand( FunctionReference.asInstructionPointer() );
}

// -----------------------------------------------------

std::string configParserStruct::callCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "CALL " << argument();
  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::callCommand* configParserStruct::callCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new callCommand( argument() );
  else
    return new ( Memory ) callCommand( argument() );
}

// =====================================================

void configParserStruct::returnCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  if ( Memory->baseStackPointer() == 0 )
  {
    Memory->popFromStack();
    Memory->jumpToNextCommand();
    return;
  }

  const variable RetValue = Memory->popFromStack();
  const variable InstructionPointer = Memory->popFromStack();
  const size_t ArgsCount = Memory->topStackValue()->ref().asArgumentsCount();
  Memory->truncateStack( Memory->stackSize() - ArgsCount - 1 );
  const variable PrevStackSize = Memory->popFromStack();

  Memory->setBaseStackPointer( PrevStackSize.ref().asStackPointer() );
  Memory->pushToStack( RetValue );
  Memory->popLocalNamedFrame();
  Memory->jumpToCommand( InstructionPointer.ref().asInstructionPointer() + 1 );
}

// -----------------------------------------------------

std::string configParserStruct::returnCommand::toString() const
{
  return "RET";
}

// -----------------------------------------------------

configParserStruct::returnCommand* configParserStruct::returnCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new returnCommand();
  else
    return new ( Memory ) returnCommand();
}

// =====================================================
   
