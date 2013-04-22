
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

unsigned configParserStruct::structParserUtil::commandsList::replaceMarker( const commandAction &Action )
{
  for ( int i = Commands.size()-1; i >= 0; i-- )
  {
    if ( Commands[i].actionType() == typeid(markerCommand) )
    {
      Commands[i] = command(Action);
      return i;
    }
  }

  return Commands.size();
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

void configParserStruct::structParserUtil::callCommand::execute( program *Program ) const
{
  assert( Program != NULL );

  const variable Function = Program->getNamedVariable( Name );
  
  if ( Function.valueType() == typeid(commandAddressVariableValue) )
  {
    unsigned NextCommandAddress = Program->currentCommandIndex() + 1;
    variable RetAddress = commandAddressVariableValue( NextCommandAddress );
    Program->pushStackVariable( RetAddress );
    Program->setCurrentCommandIndex( Function.value<commandAddressVariableValue>().address() - 1 );
    return;
  }

  if ( Function.isValueDerivedFrom<builtinFunctionValue>() )
  {
    const variable Result = Function.value<builtinFunctionValue>().execute( *Program );
    Program->pushStackVariable( Result );
    return;
  }

  Program->pushStackVariable( variable() );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::retCommand::execute( program *Program ) const
{
  const variable Result = Program->popStackVariable();
  const variable RetAddress = Program->popStackVariable();

  if ( RetAddress.valueType() == typeid(commandAddressVariableValue) )
    Program->setCurrentCommandIndex( RetAddress.value<commandAddressVariableValue>().address() - 1 );
  Program->pushStackVariable( Result );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::pushVarFrameCommand::execute( program *Program ) const
{
  Program->pushVariableFrame();
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::pushArgumentCommand::execute( program *Program ) const
{
  const variable Argument = Program->popStackVariable();
  Program->pushFunctionArgument( Argument );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::popVarFrameCommand::execute( program *Program ) const
{
  Program->popVariableFrame();
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::jumpToCommand::execute( program *Program ) const
{
  Program->setCurrentCommandIndex( Index - 1 );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::jumpIfFalseCommand::execute( program *Program ) const
{
  const variable Argument = Program->popStackVariable();
  if ( ! Argument.boolean() )
    Program->setCurrentCommandIndex( Index - 1 );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::twoStackOperandsCommand::execute( program *Program ) const
{
  const variable &Var1 = Program->popStackVariable();
  const variable &Var2 = Program->popStackVariable();
  const variable &Result = calculateResultVariable( Var1, Var2 );
  Program->pushStackVariable( Result );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::boolNotCommand::execute( program *Program ) const
{
  const variable &Var = Program->popStackVariable();
  const variable &Result = createVariable( ! Var.boolean() );
  Program->pushStackVariable( Result );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::assignVariableCommand::execute( program *Program ) const
{
  const variable Value = Program->popStackVariable();
  const variable Reference = Program->popStackVariable();

  Reference.value<referenceVariableValue>().setValue( Program, Value );
  
  Program->setNamedVariable( program::lastResultVariableName().c_str(), Value );
  Program->pushStackVariable( Value );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::assignLastExpressionCommand::execute( program *Program ) const
{
  const variable &Value = Program->topStackVariable();
  Program->setNamedVariable( program::lastResultVariableName().c_str(), Value );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::pushRefValueCommand::execute( program *Program ) const
{
  const variable Reference = Program->topStackVariable();
  variable Value = Reference.value<referenceVariableValue>().getValue( *Program );
  Program->pushStackVariable( Value );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::replaceRefToValueCommand::execute( program *Program ) const
{
  const variable Reference = Program->popStackVariable();
  variable Value = Reference.value<referenceVariableValue>().getValue( *Program );
  Program->pushStackVariable( Value );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::setRefAttributeCommand::execute( program *Program ) const
{
  variable Reference = Program->popStackVariable();
  Reference.value<referenceVariableValue>().pushAttribute( createVariable(Name) );
  Program->pushStackVariable( Reference );
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::setDictFieldCommand::execute( program *Program ) const
{
  const variable &Value = Program->popStackVariable();
  variable Dict = Program->popStackVariable();
  Dict.value<dictVariableValue>().addItem( Name, Value );
  Program->pushStackVariable( Dict );
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::pushArrayElementCommand::execute( program *Program ) const
{
  const variable &Value = Program->popStackVariable();
  variable Array = Program->popStackVariable();
  Array.value<arrayVariableValue>().pushItem( Value );
  Program->pushStackVariable( Array );
}

// =====================================================


