
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

void configParserStruct::derefCommand::exec( memory *Memory ) const
{
  assert( Memory != NULL );

  variable *Top = Memory->topStackValue();
  const variable Value = extract( Memory, Top->ref() );

  *Top = Value;
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
    size_t PrevStackSize = Memory->baseStackPointer();
    if ( ! Memory->useBaseStackPointer() )
    {
      const variable *SavedStackPointer = Memory->findStackValueByShift( PrevStackSize - 1 );
      if ( SavedStackPointer != NULL )
        PrevStackSize = SavedStackPointer->ref().asStackPointer();
    }
    const size_t Index = Reference.asArgumentIndex();
    const size_t ArgsCount = static_cast<size_t>( Memory->findStackValueByShift( PrevStackSize )->integer() );
    if ( Index == 0 )
      return variable( static_cast<int>(ArgsCount) );
    if ( Index > ArgsCount )
      return variable();
    Value = Memory->findStackValueByShift( PrevStackSize + Index );
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

std::string configParserStruct::derefCommand::toString() const
{
  return "DEREF";
}

// -----------------------------------------------------

configParserStruct::derefCommand* configParserStruct::derefCommand::clone( void *Memory ) const
{
  if ( Memory == NULL )
    return new derefCommand();
  else
    return new ( Memory ) derefCommand();
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
  Memory->setBaseStackPointer( Memory->stackSize() );
  Memory->setUseBaseStackPointer( false );
  Memory->pushToStack( variable() );
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
  
