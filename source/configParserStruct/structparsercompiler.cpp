
// =====================================================

#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparservars.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>

// =====================================================

static configParserStruct::structParserUtil::program *Program = NULL;

// =====================================================

template <class T> static inline void pushOperator()
{
  if ( Program != NULL )
    Program->pushCommand( T() );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::setStructPrserProgram( program *const Program )
{
  ::Program = Program;
}

// =====================================================

void popValueFromStack( void )
{
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::popCommand() );
}

// -----------------------------------------------------

void pushRealNumberToStack( double Number )
{
  configParserStruct::structParserUtil::variable Variable = configParserStruct::structParserUtil::createVariable( Number );
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void pushIntegerNumberToStack( int Number )
{
  pushRealNumberToStack( Number );
}

// -----------------------------------------------------

void pushStringToStack( const char *String )
{
  configParserStruct::structParserUtil::variable Variable = configParserStruct::structParserUtil::createVariable( String );
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void pushDictToStack( void ) 
{
  configParserStruct::structParserUtil::variable Variable = configParserStruct::structParserUtil::dictVariableValue(); 
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::pushValueCommand( Variable ) );
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
    Program->pushCommand( configParserStruct::structParserUtil::assignVariableCommand( StrName ) );
}

// -----------------------------------------------------

void setDictFieldFromStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( configParserStruct::structParserUtil::setDictFieldCommand( StrName ) );
}

// -----------------------------------------------------

void operatorOnStackTop( int OperatorType )
{
  using namespace configParserStruct;
  using namespace structParserUtil;

#define CASE_OF_PUSH_COMMAND( Operator, Command ) \
  case Operator: pushOperator< configParserStruct::structParserUtil::Command >(); break;

  switch ( OperatorType )
  {
    CASE_OF_PUSH_COMMAND( '+', addCommand );
    CASE_OF_PUSH_COMMAND( '-', subCommand );
    CASE_OF_PUSH_COMMAND( '*', mulCommand );
    CASE_OF_PUSH_COMMAND( '/', divCommand );
    default:
      pushOperator< configParserStruct::structParserUtil::nopCommand >();
  }

#undef PUSH_COMMAND

}

// =====================================================

