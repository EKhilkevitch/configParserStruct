
// =====================================================

#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparservars.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>

// =====================================================

static configParserStruct::structParserUtil::program *Program = NULL;

// =====================================================

void configParserStruct::structParserUtil::setStructPrserProgram( program *const Program )
{
  ::Program = Program;
}

// =====================================================

void popValueFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::popCommand() );
}

// -----------------------------------------------------

void pushRealNumberToStack( double Number )
{
  configParserStruct::structParserUtil::variable Variable = configParserStruct::structParserUtil::createVariable<double>( Number );
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::pushValueCommand( Variable ) );
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
    Program->pushCommand( configParserStruct::structParserUtil::pushVariableCommand( StrName ) );
}

// -----------------------------------------------------

void assignVariableValueFromStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::assignCommand( StrName ) );
}

// -----------------------------------------------------

void addValuesFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::addCommand() );
}

// =====================================================

