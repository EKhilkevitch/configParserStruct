
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
  if ( Program == NULL )
    throw std::invalid_argument("Program must be not NULL");
  if ( CurrentCommandIndex < Commands.size() )
    Commands.at( CurrentCommandIndex ).execute( Program );
  CurrentCommandIndex += 1;
  return CurrentCommandIndex;
}
        
// -----------------------------------------------------

void configParserStruct::structParser::commandsList::execute( program *Program )
{
  if ( Program == NULL )
    throw std::invalid_argument("Program must be not NULL");

  while ( ! isAllCommandsDone() )
    executeOneCommand( Program );
}

// =====================================================

void configParserStruct::structParser::pushValueCommand::execute( program *Program ) const
{
  assert( Program != NULL );
  Program->pushStackVariable( Variable );
}

// -----------------------------------------------------

void configParserStruct::structParser::pushVariableCommand::execute( program *Program ) const
{
  const variable &Var = Program->getNamedVariable( Name );
  Program->pushStackVariable( Var );
}

// -----------------------------------------------------

void configParserStruct::structParser::popCommand::execute( program *Program ) const
{
  Program->popStackVariable();
}

// -----------------------------------------------------

void configParserStruct::structParser::addCommand::execute( program *Program ) const
{
  const variable &Var1 = Program->popStackVariable();
  const variable &Var2 = Program->popStackVariable();
  const variable &Result = createVariable( Var1.number() + Var2.number() );
  Program->pushStackVariable( Result );
}

// -----------------------------------------------------
        
void configParserStruct::structParser::assignCommand::execute( program *Program ) const
{
  const variable &Var = Program->topStackVariable();
  Program->setNamedVariable( Name, Var );
}

// =====================================================


