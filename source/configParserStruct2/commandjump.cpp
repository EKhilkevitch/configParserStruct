
// =====================================================

#include "configParserStruct2/commandjump.h"
#include "configParserStruct2/memory.h"
#include "configParserStruct2/reference.h"
#include "configParserStruct2/buildinfunc.h"

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
  
  const variable Instruction = argument();
  const variable Value = Memory->popFromStack();
  if ( Value.boolean() )
  {
    Memory->jumpToNextCommand();
  } else {
    Memory->jumpToCommand( Instruction.ref().asInstructionPointer() );
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
  
  variable *ArgsCount = Memory->findStackValueByShift( Memory->baseStackPointer() );
  *ArgsCount = variable( static_cast<int>( Memory->stackSize() - Memory->baseStackPointer() - 1 ) );

  const char *FunctionName = argument().ref().asGlobalName();
  const variable *Function = Memory->findValueByReference( FunctionName, named::GlobalScope );
  Memory->setUseBaseStackPointer(true);

  if ( Function == NULL )
  {
    variable Result;

    const buildInFunction* BuildinFunction = Memory->findFunctionByReference( FunctionName );
    if ( BuildinFunction != NULL )
      Result = BuildinFunction->call( *Memory );

    Memory->truncateStack( Memory->baseStackPointer() );
    const variable PrevStackSize = Memory->popFromStack();
    Memory->setBaseStackPointer( PrevStackSize.ref().asStackPointer() );
    Memory->pushToStack( Result );
    Memory->jumpToNextCommand();
    return;
  }
  

  Memory->pushToStack( variable( reference( Memory->instructionPointer(), reference::InstructionPointer ) ) );
  Memory->pushLocalNamedFrame();
  Memory->jumpToCommand( Function->ref().asInstructionPointer() );
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

  const variable RetValue = Memory->popFromStack();
  const variable InstructionPointer = Memory->popFromStack();

  Memory->truncateStack( Memory->baseStackPointer() );
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
   
