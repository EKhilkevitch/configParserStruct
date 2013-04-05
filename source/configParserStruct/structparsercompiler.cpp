
// =====================================================

#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparservars.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>

// =====================================================

static configParserStruct::structParser::program *Program = NULL;

// =====================================================

void configParserStruct::structParser::setStructPrserProgram( program *const Program )
{
  ::Program = Program;
}

// =====================================================

void popValueFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::popCommand() );
}

// -----------------------------------------------------

void pushRealNumberToStack( double Number )
{
  configParserStruct::structParser::variable Variable = configParserStruct::structParser::createVariable<double>( Number );
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void pushIntegerNumberToStack( int Number )
{
  pushRealNumberToStack( Number );
}

// -----------------------------------------------------

void pushVariableValueToStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::pushVariableCommand( StrName ) );
}

// -----------------------------------------------------

void assignVariableValueFromStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::assignCommand( StrName ) );
}

// -----------------------------------------------------

void addValuesFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParser::addCommand() );
}

// =====================================================

