
// =====================================================

#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparservars.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
  
extern "C" int CPSSPU_lexCurrentLineNumber();

using namespace configParserStruct::structParserUtil;

// =====================================================

static program *Program = NULL;

// =====================================================

void configParserStruct::structParserUtil::setStructPrserProgram( program *const Program )
{
  ::Program = Program;
}

// =====================================================

void CPSSPU_popValueFromStack( void )
{
  if ( Program != NULL )
    Program->pushCommand( popCommand() );
}

// -----------------------------------------------------

void CPSSPU_pushRealNumberToStack( double Number )
{
  variable Variable = createVariable( Number );
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void CPSSPU_pushIntegerNumberToStack( int Number )
{
  variable Variable = createVariable( Number );
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void CPSSPU_pushStringToStack( const char *String )
{
  variable Variable = createVariable( String );
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void CPSSPU_pushDictToStack( void ) 
{
  variable Variable = dictVariableValue(); 
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void CPSSPU_pushArrayToStack( void )
{
  variable Variable = arrayVariableValue(); 
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( Variable ) );
}

// -----------------------------------------------------

void CPSSPU_pushVariableValueToStack( void )
{
  if ( Program != NULL )
    Program->pushCommand( pushRefValueCommand() );
}

// -----------------------------------------------------

void CPSSPU_replaceReferenceToValueOnStack( void )
{
  if ( Program != NULL )
    Program->pushCommand( replaceRefToValueCommand() );
}

// -----------------------------------------------------

void CPSSPU_pushVariableReferenceToStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( pushValueCommand( referenceVariableValue(StrName) ) );
}

// -----------------------------------------------------

void CPSSPU_assignVariableValueFromStack( void )
{
  if ( Program != NULL )
    Program->pushCommand( assignVariableCommand() );
}

// -----------------------------------------------------

void CPSSPU_setDictFieldFromStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? std::string() : Name;
  if ( Program != NULL )
    Program->pushCommand( setDictFieldCommand( StrName ) );
}

// -----------------------------------------------------

void CPSSPU_setArrayElementFromStack( void )
{
  if ( Program != NULL )
    Program->pushCommand( pushArrayElementCommand() );
}

// -----------------------------------------------------

void CPSSPU_operatorOnStackTop( const char *OperatorType )
{
  if ( Program == NULL || OperatorType == NULL )
    return;

#define CASE_OF_PUSH_COMMAND( Operator, Command ) \
  do { if ( std::strcmp( Operator, OperatorType ) == 0 ) { Program->pushCommand( Command() ); return; } } while (false)

  CASE_OF_PUSH_COMMAND( "+",  addCommand );
  CASE_OF_PUSH_COMMAND( "-",  subCommand );
  CASE_OF_PUSH_COMMAND( "*",  mulCommand );
  CASE_OF_PUSH_COMMAND( "/",  divCommand );
  CASE_OF_PUSH_COMMAND( "%",  modCommand );

  CASE_OF_PUSH_COMMAND( "==", numEqCommand );
  CASE_OF_PUSH_COMMAND( "!=", numNeCommand );
  CASE_OF_PUSH_COMMAND( ">",  numGtCommand );
  CASE_OF_PUSH_COMMAND( "<",  numLtCommand );
  CASE_OF_PUSH_COMMAND( ">=", numGeCommand );
  CASE_OF_PUSH_COMMAND( "<=", numLeCommand );
  
  CASE_OF_PUSH_COMMAND( "&&",  boolAndCommand );
  CASE_OF_PUSH_COMMAND( "||",  boolOrCommand );
  CASE_OF_PUSH_COMMAND( "!",   boolNotCommand );

#undef PUSH_COMMAND
}

// -----------------------------------------------------

void CPSSPU_finalizeExpressionStack( void )
{
  if ( Program != NULL )
    Program->pushCommand( assignLastExpressionCommand() );
  CPSSPU_popValueFromStack();
}

// -----------------------------------------------------

void CPSSPU_beginOfNewFunctionAssignName( void )
{
  if ( Program == NULL )
    return;
  unsigned Index = Program->numberOfCommands();
  Program->pushCommand( pushValueCommand( commandAddressVariableValue( Index+4 ) ) );
  Program->pushCommand( assignVariableCommand() );
  Program->pushCommand( popCommand() );
  Program->pushCommand( markerCommand() );
//  Program->setNamedVariable( StrName, commandAddressVariableValue( Index+1 ) );
}

// -----------------------------------------------------

void CPSSPU_endOfCurrentFunction( void )
{
  if ( Program == NULL )
    return;
  Program->pushCommand( pushValueCommand( variable() ) );
  Program->pushCommand( retCommand() );
  Program->replaceCommandMarkerToJmp<jumpToCommand>();
}

// -----------------------------------------------------

void CPSSPU_returnFromCurrentFunction( void )
{
  if ( Program == NULL )
    return;
  Program->pushCommand( retCommand() );
}

// -----------------------------------------------------

void CPSSPU_prepareToFunctionCall( void )
{
  if ( Program != NULL )
    Program->pushCommand( pushVarFrameCommand() );
}

// -----------------------------------------------------

void CPSSPU_callFunctionWithArgsFromStack( const char *Name )
{
  std::string StrName = ( Name == NULL ) ? "" : Name;
  if ( Program != NULL )
  {
    Program->pushCommand( callCommand( StrName ) );
    Program->pushCommand( popVarFrameCommand() );
  }
}

// -----------------------------------------------------

void CPSSPU_pushFunctionArgument( void )
{
  if ( Program != NULL )
    Program->pushCommand( pushArgumentCommand() );
}

// -----------------------------------------------------

void CPSSPU_beginOfIfStatement( void )
{
  if ( Program != NULL )
    Program->pushCommand( markerCommand() );
}

// -----------------------------------------------------

void CPSSPU_beginOfElseStatement( void )
{
  if ( Program != NULL )
  {
    Program->replaceCommandMarkerToJmp<jumpIfFalseCommand>(+1);
    Program->pushCommand( markerCommand() );
  }
}

// -----------------------------------------------------

void CPSSPU_endOfIfStatement( void )
{
  if ( Program != NULL )
    Program->replaceCommandMarkerToJmp<jumpIfFalseCommand>();
}

// -----------------------------------------------------

void CPSSPU_endOfIfElseStatement( void )
{
  if ( Program != NULL )
    Program->replaceCommandMarkerToJmp<jumpToCommand>();
}

// -----------------------------------------------------

void CPSSPU_prepareForWhileStatement( void )
{
  if ( Program != NULL )
    Program->pushCommand( markerCommand() );
}

// -----------------------------------------------------

void CPSSPU_beginOfWhileStatement( void )
{
  if ( Program != NULL )
    Program->pushCommand( markerCommand() );
}

// -----------------------------------------------------

void CPSSPU_endOfWhileStatement( void )
{
  if ( Program != NULL )
  {
    unsigned IndexIf = Program->replaceCommandMarkerToJmp<jumpIfFalseCommand>(+1);
    unsigned IndexBegin = Program->replaceCommandMarkerToNop();
    Program->pushCommand( jumpToCommand(IndexBegin) );
  }
}

// -----------------------------------------------------

void CPSSPU_setStructParserError( void )
{
  if ( Program != NULL && Program->errorLine() < 0 )
  {
    int LineNumber = CPSSPU_lexCurrentLineNumber();
    Program->setErrorLine( LineNumber );
  }
}

// =====================================================

