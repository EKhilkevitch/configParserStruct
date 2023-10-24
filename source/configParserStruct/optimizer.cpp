
// =====================================================

#include "configParserStruct/optimizer.h"
#include "configParserStruct/text.h"
#include "configParserStruct/command.h"
#include "configParserStruct/variable.h"
#include "configParserStruct/reference.h"

#include <limits>
#include <stdexcept>
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

// =====================================================
      
