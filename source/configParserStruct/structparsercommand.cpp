
// =====================================================

#include "configParserStruct/structparsercommand.h"
#include "configParserStruct/structparserprogram.h"

#include <stdexcept>
#include <cassert>

// =====================================================

unsigned configParserStruct::structParser::commandsList::push( const command &Cmd )
{
  Commands.push_back( Cmd );
  return Commands.size() - 1;
}

// -----------------------------------------------------

unsigned configParserStruct::structParser::commandsList::push( const commandAction &Action )
{
  command Cmd(Action);
  return push(Cmd);
}

// -----------------------------------------------------

void configParserStruct::structParser::commandsList::set( unsigned Index, const command &Cmd )
{
  Commands.at(Index) = Cmd;
}

// -----------------------------------------------------

const configParserStruct::structParser::command& configParserStruct::structParser::commandsList::currentCommand() const
{
  static const command NopCommand;
  if ( CurrentCommandIndex < Commands.size() )
    return Commands.at( CurrentCommandIndex );
  return NopCommand;
}

// -----------------------------------------------------
        
unsigned configParserStruct::structParser::commandsList::executeOneCommand( program *Program )
{
  if ( CurrentCommandIndex < Commands.size() )
    Commands.at( CurrentCommandIndex ).execute( Program );
  CurrentCommandIndex += 1;
  return CurrentCommandIndex;
}

// =====================================================

void configParserStruct::structParser::pushCommand::execute( program *Program ) const
{
  assert( Program != NULL );
  Program->pushVariable( Variable );
}

// -----------------------------------------------------

void configParserStruct::structParser::popCommand::execute( program *Program ) const
{
  Program->popVariable();
}

// -----------------------------------------------------

void configParserStruct::structParser::addCommand::execute( program *Program ) const
{
  const variable &Var1 = Program->popVariable();
  const variable &Var2 = Program->popVariable();
  const variable &Result = createVariable( Var1.number() + Var2.number() );
  Program->pushVariable( Result );
}

// =====================================================


