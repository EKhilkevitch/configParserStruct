
// =====================================================

#include "configParserStruct/structparsercompiler.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>

// =====================================================

static configParserStruct::structParser::program *Program = NULL;

// =====================================================

void popValueFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::popCommand() );
}

// -----------------------------------------------------

void pushRealNumberToStack( double Number )
{
  configParserStruct::structParser::realVariableValue Value( Number );
  configParserStruct::structParser::variable Variable( Value );
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::pushCommand( Variable ) );
}

// -----------------------------------------------------

void pushIntegerNumberToStack( int Number )
{
  pushRealNumberToStack( Number );
}

// -----------------------------------------------------

void addValuesFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::addCommand() );
}

// =====================================================

configParserStruct::structParser::program::program()
{
}

// -----------------------------------------------------

configParserStruct::structParser::program::~program()
{
}

// -----------------------------------------------------

unsigned configParserStruct::structParser::program::pushCommand( const commandAction &A ) 
{ 
  command Cmd( A );
  Commands.push_back( Cmd ); 
}

// -----------------------------------------------------

void configParserStruct::structParser::program::pushVariable( const variableValue &V ) 
{ 
  variable Var( V );
  pushVariable( Var ); 
}

// -----------------------------------------------------

void configParserStruct::structParser::program::pushVariable( const variable &Var ) 
{ 
  Stack.push_back( Var ); 
}

// -----------------------------------------------------

const configParserStruct::structParser::variable configParserStruct::structParser::program::popVariable()
{
  if ( Stack.empty() )
    return variable();

  variable Variable = Stack.back();
  Stack.pop_back();
  return Variable;
}

// =====================================================

