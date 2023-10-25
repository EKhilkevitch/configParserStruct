
// =====================================================

#include "configParserStruct/optimizer.h"
#include "configParserStruct/text.h"
#include "configParserStruct/command.h"
#include "configParserStruct/commandbinops.h"
#include "configParserStruct/commandstack.h"
#include "configParserStruct/commandjump.h"
#include "configParserStruct/variable.h"
#include "configParserStruct/variablevalue.h"
#include "configParserStruct/reference.h"

#include <limits>
#include <stdexcept>
#include <typeinfo>
#include <cassert>

// =====================================================
        
configParserStruct::optimizer::jumpCommandInfo::jumpCommandInfo() :
  Index( std::numeric_limits<size_t>::max() ),
  Destination( std::numeric_limits<size_t>::max() )
{
}

// -----------------------------------------------------

configParserStruct::optimizer::jumpCommandInfo::jumpCommandInfo( size_t I, size_t D ) :
  Index( I ),
  Destination( D )
{
}

// =====================================================

configParserStruct::optimizer::optimizer()
{
}

// -----------------------------------------------------

void configParserStruct::optimizer::operator()( text *Text ) const
{
  return optimize(Text);
}

// -----------------------------------------------------

void configParserStruct::optimizer::optimize( text *Text ) const
{
  if ( Text == NULL )
    throw std::invalid_argument( "Text must be not NULL" );

  optimizeUminus( Text );
  optimizeAssign( Text );
}

// -----------------------------------------------------
      
std::vector< configParserStruct::optimizer::jumpCommandInfo > configParserStruct::optimizer::jumpCommands( const text &Text )
{
  const size_t TextSize = Text.size();

  std::vector< jumpCommandInfo > Result;
  Result.reserve( TextSize / 4 );

  for ( size_t Index = 0; Index < TextSize; Index++ )
  {
    const command &Command = Text[Index];
    const variable &Argument = Command.argument();
    const reference &Reference = Argument.ref();
    if ( Reference.hasType( reference::InstructionPointer ) )
      Result.push_back( jumpCommandInfo( Index, Reference.asInstructionPointer() ) );
  }

  return Result;
}

// -----------------------------------------------------
      
const configParserStruct::command* configParserStruct::optimizer::nextEffectiveCommand( const text &Text, size_t *Index )
{
  assert( Index != NULL );

  const size_t TextSize = Text.size();
  
  while ( *Index < TextSize )
  {
    const command &Command = Text[*Index];
    (*Index) += 1;
    if ( typeid(Command) != typeid(nopCommand) )
      return &Command;
  }

  return NULL;
}

// -----------------------------------------------------
      
void configParserStruct::optimizer::optimizeUminus( text *Text )
{
  assert( Text != NULL );
  
  size_t Index = 0;
  while ( true )
  {
    const command *FirstCommand = nextEffectiveCommand( *Text, &Index );
    const size_t FirstIndex = Index - 1;
    if ( FirstCommand == NULL )
      break;

    if ( typeid(*FirstCommand) != typeid(pushValueCommand) )
      continue;

    const command *SecondCommand = nextEffectiveCommand( *Text, &Index );
    const size_t SecondIndex = Index - 1;
    if ( SecondCommand == NULL )
      break;

    if ( typeid(*SecondCommand) != typeid(unminusCommand) )
      continue;

    const variable &Value = FirstCommand->argument();
    const char *ValueType = Value.type();
    
    if ( ValueType == integerVariableValue::TypeName )
    {
      Text->replaceCommand( FirstIndex, pushValueCommand(-Value.integer()) );
      Text->replaceCommand( SecondIndex, nopCommand() );
      continue;
    }

    if ( ValueType == realVariableValue::TypeName )
    {
      Text->replaceCommand( FirstIndex, pushValueCommand(-Value.real()) );
      Text->replaceCommand( SecondIndex, nopCommand() );
      continue;
    }

  }
}

// -----------------------------------------------------
      
void configParserStruct::optimizer::optimizeAssign( text *Text )
{
  assert( Text != NULL );

  size_t Index = 0;
  while ( true )
  {
    const command *FirstCommand = nextEffectiveCommand( *Text, &Index );
    const size_t FirstIndex = Index - 1;
    if ( FirstCommand == NULL )
      break;

    if ( typeid(*FirstCommand) != typeid(pushValueCommand) )
      continue;

    const command *SecondCommand = nextEffectiveCommand( *Text, &Index );
    const size_t SecondIndex = Index - 1;
    if ( SecondCommand == NULL )
      break;

    if ( typeid(*SecondCommand) != typeid(assignCommand) )
      continue;

    const variable &Value = FirstCommand->argument();
    Text->replaceCommand( FirstIndex, assignCommand(Value) );
    Text->replaceCommand( SecondIndex, nopCommand() );
    continue;

  }
}

// =====================================================
      
