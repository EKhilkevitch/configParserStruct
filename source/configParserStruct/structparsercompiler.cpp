
// =====================================================

#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserprogram.h"

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

