
// =====================================================

#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparservars.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
  
extern "C" int lexCurrentLineNumber();

using namespace configParserStruct::structParserUtil;

// =====================================================

static program *Program = NULL;

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
    Program->pushCommand( popCommand() );
}

// -----------------------------------------------------

void pushRealNumberToStack( double Number )
{
  variable Variable = createVariable( Number );
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void pushIntegerNumberToStack( int Number )
{
  pushRealNumberToStack( Number );
}

// -----------------------------------------------------

void pushStringToStack( const char *String )
{
  variable Variable = createVariable( String );
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void pushDictToStack( void ) 
{
  variable Variable = dictVariableValue(); 
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void pushVariableValueToStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( pushVariableCommand( StrName ) );
}

// -----------------------------------------------------

void assignVariableValueFromStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( assignVariableCommand( StrName ) );
}

// -----------------------------------------------------

void setDictFieldFromStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( setDictFieldCommand( StrName ) );
}

// -----------------------------------------------------

void operatorOnStackTop( int OperatorType )
{
  using namespace configParserStruct;
  using namespace structParserUtil;

#define CASE_OF_PUSH_COMMAND( Operator, Command ) \
  case Operator: pushOperator< Command >(); break;

  switch ( OperatorType )
  {
    CASE_OF_PUSH_COMMAND( '+', addCommand );
    CASE_OF_PUSH_COMMAND( '-', subCommand );
    CASE_OF_PUSH_COMMAND( '*', mulCommand );
    CASE_OF_PUSH_COMMAND( '/', divCommand );
    default:
      pushOperator< nopCommand >();
  }

#undef PUSH_COMMAND
}

// -----------------------------------------------------

void finalizeExpressionStack( void )
{
  assignVariableValueFromStack( program::lastResultVariableName().c_str() );
  popValueFromStack();
}

// -----------------------------------------------------

void setStructParserError( void )
{
  if ( Program != NULL && Program->errorLine() < 0 )
  {
    int LineNumber = lexCurrentLineNumber();
    Program->setErrorLine( LineNumber );
  }
}

// =====================================================

