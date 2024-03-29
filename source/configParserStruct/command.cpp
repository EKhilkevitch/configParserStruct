
// =====================================================

#include "configParserStruct/command.h"
#include "configParserStruct/memory.h"

#include <ostream>
#include <cassert>

// =====================================================
      
configParserStruct::command::command()
{
}

// -----------------------------------------------------

configParserStruct::command::command( const variable &V ) :
  Argument(V)
{
}

// -----------------------------------------------------

configParserStruct::command::~command()
{
}

// -----------------------------------------------------

const configParserStruct::variable& configParserStruct::command::argument() const
{
  return Argument;
}

// -----------------------------------------------------
      
configParserStruct::command* configParserStruct::command::cloneWithArgument( const variable &Variable, void *Memory ) const
{
  command *Clone = clone( Memory );
  assert( Clone != NULL );
  Clone->Argument = Variable;
  return Clone;
}

// =====================================================

std::ostream& configParserStruct::operator<<( std::ostream &Stream, const command &Command )
{
  const void *Pointer = &Command;
  bool IsPlaceholder = true;
  for ( size_t i = 0; i < sizeof(command) && IsPlaceholder; i++ )
  {
    if ( static_cast<const char*>(Pointer)[i] != 0 )
      IsPlaceholder = false;
  }

  if ( IsPlaceholder )
    return Stream << "(PLACEHOLDER)";
  else
    return Stream << Command.toString();
}

// =====================================================
      
