
// =====================================================

#include "configParserStruct/commandstack.h"
#include "configParserStruct/memory.h"
#include "configParserStruct/reference.h"

#include <sstream>
#include <cassert>

// =====================================================

configParserStruct::pushValueCommand::pushValueCommand()
{
}

// -----------------------------------------------------

configParserStruct::pushValueCommand::pushValueCommand( const variable &Variable ) :
  command( variable(Variable) )
{
}

// -----------------------------------------------------

configParserStruct::pushValueCommand::pushValueCommand( int Integer ) :
  command( variable(Integer) )
{
  assert( sizeof(pushValueCommand) <= sizeof(command) );
}

// -----------------------------------------------------

configParserStruct::pushValueCommand::pushValueCommand( double Real ) :
  command( variable(Real) )
{
  assert( sizeof(pushValueCommand) <= sizeof(command) );
}

// -----------------------------------------------------

configParserStruct::pushValueCommand::pushValueCommand( const char *String ) :
  command( variable(String) )
{
  assert( sizeof(pushValueCommand) <= sizeof(command) );
}

// -----------------------------------------------------
      
configParserStruct::pushValueCommand::pushValueCommand( variable::collectionType Type ) :
  command( variable(Type) )
{
}

// -----------------------------------------------------

void configParserStruct::pushValueCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  Memory->pushToStack( argument() );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::pushValueCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "PUSH " << argument();
  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::pushValueCommand* configParserStruct::pushValueCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new pushValueCommand( argument() );
  else
    return new ( Memory ) pushValueCommand( argument() );
}

// =====================================================

configParserStruct::pushLocalDataRefCommand::pushLocalDataRefCommand( const variable &Variable ) :
  command( variable(Variable) )
{
}

// -----------------------------------------------------

configParserStruct::pushLocalDataRefCommand::pushLocalDataRefCommand( const char *Name ) :
  command( variable( reference(Name,reference::LocalName) ) )
{
}

// -----------------------------------------------------

configParserStruct::pushLocalDataRefCommand::pushLocalDataRefCommand( size_t Index ) :
  command( variable( reference(Index,reference::ArgumentIndex) ) )
{
}

// -----------------------------------------------------

void configParserStruct::pushLocalDataRefCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  Memory->pushToStack( argument() );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::pushLocalDataRefCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "PUSH LOCAL DATA REF " << argument();
  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::pushLocalDataRefCommand* configParserStruct::pushLocalDataRefCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new pushLocalDataRefCommand( argument() );
  else
    return new ( Memory ) pushLocalDataRefCommand( argument() );
}

// =====================================================

configParserStruct::pushGlobalDataRefCommand::pushGlobalDataRefCommand( const variable &Variable ) :
  command( variable(Variable) )
{
}

// -----------------------------------------------------

configParserStruct::pushGlobalDataRefCommand::pushGlobalDataRefCommand( const char *Name ) :
  command( variable( reference(Name,reference::GlobalName) ) )
{
}

// -----------------------------------------------------

void configParserStruct::pushGlobalDataRefCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  Memory->pushToStack( argument() );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::pushGlobalDataRefCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "PUSH GLOBAL DATA REF " << argument();
  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::pushGlobalDataRefCommand* configParserStruct::pushGlobalDataRefCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new pushGlobalDataRefCommand( argument() );
  else
    return new ( Memory ) pushGlobalDataRefCommand( argument() );
}

// =====================================================

configParserStruct::pushInstructionRefCommand::pushInstructionRefCommand( const variable &Variable ) :
  command( variable(Variable) )
{
}

// -----------------------------------------------------

configParserStruct::pushInstructionRefCommand::pushInstructionRefCommand( size_t Index ) :
  command( variable( reference( Index,reference::InstructionPointer) ) )
{
}

// -----------------------------------------------------

void configParserStruct::pushInstructionRefCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  Memory->pushToStack( argument() );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::pushInstructionRefCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "PUSH INSTR REF " << argument();
  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::pushInstructionRefCommand* configParserStruct::pushInstructionRefCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new pushInstructionRefCommand( argument() );
  else
    return new ( Memory ) pushInstructionRefCommand( argument() );
}

// =====================================================

void configParserStruct::assignCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Value = Memory->popFromStack();
  variable *Top = Memory->topStackValue();
  
  assign( Memory, Top->ref(), Value );
  *Top = Value;

  Memory->jumpToNextCommand();
}

// -----------------------------------------------------
      
void configParserStruct::assignCommand::assign( memory *Memory, const reference &Reference, const variable &Value )
{
  const reference *Next = Reference.next();

  if ( Next == NULL )
  {
    if ( Reference.hasType( reference::LocalName ) )
    {
      Memory->setValueByReference( Reference.asLocalName(), Value, named::LocalScope );
    } else if ( Reference.hasType( reference::GlobalName ) ) {
      Memory->setValueByReference( Reference.asGlobalName(), Value, named::GlobalScope );
    } else if ( Reference.hasType( reference::StackPointer ) ) {
      variable *Pointer = Memory->findStackValueByShift( Memory->baseStackPointer() + Reference.asStackPointer() );
      if ( Pointer != NULL )
        *Pointer = Value;
    } else {
      assert( false );
    }
    return;
  }

  variable *Base = NULL;
  if ( Reference.hasType( reference::LocalName ) )
  {
    Base = Memory->findValueByReference( Reference.asLocalName(), named::LocalScope );
    if ( Base == NULL )
      Base = Memory->setValueByReference( Reference.asLocalName(), createEmpty(*Next), named::LocalScope );
  } else if ( Reference.hasType( reference::GlobalName ) ) {
    Base = Memory->findValueByReference( Reference.asGlobalName(), named::GlobalScope );
    if ( Base == NULL )
      Base = Memory->setValueByReference( Reference.asGlobalName(), createEmpty(*Next), named::GlobalScope );
  } else {
    assert( false );
    return;
  }

  assert( Base != NULL && Next != NULL );

  while ( true )
  {
    variable *Secondary = Base->getByRef( *Next );
    if ( Secondary == NULL )
    {
      *Base = createEmpty(*Next);
      Secondary = Base->getByRef( *Next );
      assert( Secondary != NULL );
    }

    if ( Next->next() == NULL )
    {
      *Secondary = Value;
      break;
    } 
      
    Next = Next->next();
    Base = Secondary;
  }
}

// -----------------------------------------------------
      
configParserStruct::variable configParserStruct::assignCommand::createEmpty( const reference &Reference )
{
  if ( Reference.hasType( reference::ArrayIndex ) )
    return variable( variable::ArrayCollection );
  if ( Reference.hasType( reference::DictKey ) )
    return variable( variable::DictCollection );

  assert( false );
  return variable();
}

// -----------------------------------------------------

std::string configParserStruct::assignCommand::toString() const
{
  return "ASSIGN";
}

// -----------------------------------------------------

configParserStruct::assignCommand* configParserStruct::assignCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new assignCommand();
  else
    return new ( Memory ) assignCommand();
}

// =====================================================
      
configParserStruct::derefCommand::derefCommand( const variable &Variable ) :
  command(Variable)
{
}

// -----------------------------------------------------

configParserStruct::derefCommand::derefCommand( type Type ) :
  command( variable( static_cast<int>(Type) ) )
{
}

// -----------------------------------------------------
      
configParserStruct::derefCommand::derefCommand( const reference &Reference ) :
  command( variable(Reference) )
{
}

// -----------------------------------------------------

void configParserStruct::derefCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const reference &ArgRef = argument().ref();
  if ( ArgRef.hasType( reference::None ) )
  {
    variable *Top = Memory->topStackValue();
    const variable Value = extract( Memory, Top->ref() );
    *Top = Value;
  } else {
    const variable Value = extract( Memory, ArgRef );
    Memory->pushToStack( Value );
  }
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------
      
configParserStruct::variable configParserStruct::derefCommand::extract( memory *Memory, const reference &Reference ) 
{
  const variable *Value = NULL;

  if ( Reference.hasType( reference::LocalName ) )
  {
    Value = Memory->findValueByReference( Reference.asLocalName(), named::LocalScope );
  } else if ( Reference.hasType( reference::GlobalName ) ) {
    Value = Memory->findValueByReference( Reference.asGlobalName(), named::GlobalScope );
  } else if ( Reference.hasType( reference::ArgumentIndex ) ) {
    const size_t BaseStackPointer = Memory->baseStackPointer();
    const size_t ArgsCount = Memory->findStackValueByShift( BaseStackPointer - 2 )->ref().asArgumentsCount();
    const size_t Index = Reference.asArgumentIndex();
    if ( Index == 0 )
      return variable( static_cast<int>(ArgsCount) );
    if ( Index > ArgsCount )
      return variable();
    Value = Memory->findStackValueByShift( BaseStackPointer - ArgsCount - 3 + Index );
  }

  const reference *Next = Reference.next();
  while ( Next != NULL && Value != NULL )
  {
    Value = Value->getByRef( *Next );
    Next = Next->next();
  }

  if ( Value == NULL )
    return variable();

  return *Value;
}

// -----------------------------------------------------
      
size_t configParserStruct::derefCommand::extractArgsCount( const memory &Memory )
{
  const size_t BaseStackPointer = Memory.baseStackPointer();
  const size_t ArgsCount = Memory.findStackValueByShift( BaseStackPointer - 2 )->ref().asArgumentsCount();
  return ArgsCount;
}

// -----------------------------------------------------

const configParserStruct::variable& configParserStruct::derefCommand::extractArgValue( const memory &Memory, size_t Index, size_t ArgsCount )
{
  const size_t BaseStackPointer = Memory.baseStackPointer();
  assert( Index < ArgsCount );
  return *Memory.findStackValueByShift( BaseStackPointer - ArgsCount - 2 + Index );
}

// -----------------------------------------------------

std::string configParserStruct::derefCommand::toString() const
{
  std::ostringstream Stream;
  if ( argument().ref().hasType(reference::None) )
    Stream << "DEREF FROM STACK";
  else
    Stream << "DEREF " << argument();
  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::derefCommand* configParserStruct::derefCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new derefCommand( argument() );
  else
    return new ( Memory ) derefCommand( argument() );
}

// =====================================================
      
configParserStruct::joinrefCommand::joinrefCommand( type Type ) :
  command( variable( static_cast<int>(Type) ) )
{
}

// -----------------------------------------------------

void configParserStruct::joinrefCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Secondary = Memory->popFromStack();
  variable *Base = Memory->topStackValue();

  reference Result = Base->ref();
  const type Type = static_cast<type>( argument().integer() );

  switch ( Type )
  {
    case Array:
      {
        const reference SecondaryAsRef( Secondary.integer(), reference::ArrayIndex );
        Result.setAsTail( SecondaryAsRef );
      }
      break;

    case Dict:
      {
        const reference SecondaryAsRef( Secondary.string().c_str(), reference::DictKey );
        Result.setAsTail( SecondaryAsRef );
      }
      break;

    default:
      assert( false );
      break;
  }
  
  *Base = variable(Result);
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::joinrefCommand::toString() const
{
  return "JOINREF";
}

// -----------------------------------------------------

configParserStruct::joinrefCommand* configParserStruct::joinrefCommand::clone( void *Memory ) const
{
  const type Type = static_cast<type>( argument().integer() );
  if ( Memory == NULL )
    return new joinrefCommand( Type );
  else
    return new ( Memory ) joinrefCommand( Type );
}

// =====================================================

void configParserStruct::duptopCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable *Top = Memory->topStackValue();
  Memory->pushToStack( *Top );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::duptopCommand::toString() const
{
  return "DUPTOP";
}

// -----------------------------------------------------

configParserStruct::duptopCommand* configParserStruct::duptopCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new duptopCommand();
  else
    return new ( Memory ) duptopCommand();
}

// =====================================================

void configParserStruct::popAndAssignLastCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const variable Var = Memory->popFromStack();
  Memory->setLastResult( Var );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::popAndAssignLastCommand::toString() const
{
  return "POP_ASSIGN";
}

// -----------------------------------------------------

configParserStruct::popAndAssignLastCommand* configParserStruct::popAndAssignLastCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new popAndAssignLastCommand();
  else
    return new ( Memory ) popAndAssignLastCommand();
}

// =====================================================

void configParserStruct::pushStackSizeCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  const size_t PrevBaseStackPointer = Memory->baseStackPointer();
  Memory->pushToStack( variable( reference( PrevBaseStackPointer, reference::StackPointer ) ) );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::pushStackSizeCommand::toString() const
{
  return "PUSH SP";
}

// -----------------------------------------------------

configParserStruct::pushStackSizeCommand* configParserStruct::pushStackSizeCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new pushStackSizeCommand();
  else
    return new ( Memory ) pushStackSizeCommand();
}

// =====================================================

configParserStruct::pushArgsCountCommand::pushArgsCountCommand( const variable &Variable ) :
  command(Variable)
{
}

// -----------------------------------------------------

configParserStruct::pushArgsCountCommand::pushArgsCountCommand( size_t Count ) :
  command( variable( reference( Count, reference::ArgumentsCount ) ) )
{
}

// -----------------------------------------------------

void configParserStruct::pushArgsCountCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );
  Memory->pushToStack( argument() );
  Memory->jumpToNextCommand();
}

// -----------------------------------------------------

std::string configParserStruct::pushArgsCountCommand::toString() const
{
  std::ostringstream Stream;
  Stream << "PUSH AC " << argument();
  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::pushArgsCountCommand* configParserStruct::pushArgsCountCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new pushArgsCountCommand( argument() );
  else
    return new ( Memory ) pushArgsCountCommand( argument() );
}

// =====================================================
  
