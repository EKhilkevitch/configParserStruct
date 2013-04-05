
// =====================================================

#include "configParserStruct/structparsercommand.h"
#include "configParserStruct/structparserprogram.h"

#include <stdexcept>
#include <cassert>

// =====================================================

unsigned configParserStruct::structParserUtil::commandsList::push( const command &Cmd )
{
  Commands.push_back( Cmd );
  return Commands.size() - 1;
}

// -----------------------------------------------------

unsigned configParserStruct::structParserUtil::commandsList::push( const commandAction &Action )
{
  command Cmd(Action);
  return push(Cmd);
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::commandsList::set( unsigned Index, const command &Cmd )
{
  Commands.at(Index) = Cmd;
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::command& configParserStruct::structParserUtil::commandsList::currentCommand() const
{
  static const command NopCommand;
  if ( CurrentCommandIndex < Commands.size() )
    return Commands.at( CurrentCommandIndex );
  return NopCommand;
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::commandsList::clear()
{
  Commands.clear();
  CurrentCommandIndex = 0;
}

// -----------------------------------------------------
        
unsigned configParserStruct::structParserUtil::commandsList::executeOneCommand( program *Program )
{
  if ( Program == NULL )
    throw std::invalid_argument("Program must be not NULL");
  if ( CurrentCommandIndex < Commands.size() )
    Commands.at( CurrentCommandIndex ).execute( Program );
  CurrentCommandIndex += 1;
  return CurrentCommandIndex;
}
        
// -----------------------------------------------------

void configParserStruct::structParserUtil::commandsList::execute( program *Program )
{
  if ( Program == NULL )
    throw std::invalid_argument("Program must be not NULL");

  while ( ! isAllCommandsDone() )
    executeOneCommand( Program );
}

// =====================================================

void configParserStruct::structParserUtil::pushValueCommand::execute( program *Program ) const
{
  assert( Program != NULL );
  Program->pushStackVariable( Variable );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::pushVariableCommand::execute( program *Program ) const
{
  const variable &Var = Program->getNamedVariable( Name );
  Program->pushStackVariable( Var );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::popCommand::execute( program *Program ) const
{
  Program->popStackVariable();
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::addCommand::execute( program *Program ) const
{
  const variable &Var1 = Program->popStackVariable();
  const variable &Var2 = Program->popStackVariable();
  const variable &Result = createVariable( Var1.number() + Var2.number() );
  Program->pushStackVariable( Result );
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::assignCommand::execute( program *Program ) const
{
  const variable &Var = Program->topStackVariable();
  Program->setNamedVariable( Name, Var );
}

// =====================================================


