
// =====================================================

#include "configParserStruct/bisoncompiler.h"
#include "configParserStruct/text.h"
#include "configParserStruct/reference.h"
#include "configParserStruct/commandbinops.h"
#include "configParserStruct/commandcmp.h"
#include "configParserStruct/commandjump.h"
#include "configParserStruct/commandstack.h"

#include <iostream>
#include <typeinfo>
#include <cstring>
#include <cassert>

// =====================================================

extern "C" size_t CPSSPU_lexCurrentLineNumber( void );

// =====================================================

static configParserStruct::text *Text = NULL;

// -----------------------------------------------------

void CPSSPU_setTextForBison( configParserStruct::text *T )
{
  Text = T;
}

// =====================================================

void CPSSPU_pushRealNumberToStack( double Number ) 
{
  assert( Text != NULL );
  Text->push( configParserStruct::pushValueCommand(Number) );
}

// -----------------------------------------------------

void CPSSPU_pushIntegerNumberToStack( int Number ) 
{
  assert( Text != NULL );
  Text->push( configParserStruct::pushValueCommand(Number) );
}

// -----------------------------------------------------

void CPSSPU_pushStringToStack( const char *String ) 
{
  assert( Text != NULL );
  assert( String != NULL );
  Text->push( configParserStruct::pushValueCommand(String) );
}

// -----------------------------------------------------

void CPSSPU_pushDictToStack( void ) 
{
  Text->push( configParserStruct::pushValueCommand( configParserStruct::variable::DictCollection ) );
}

// -----------------------------------------------------

void CPSSPU_pushArrayToStack( void ) 
{
  Text->push( configParserStruct::pushValueCommand( configParserStruct::variable::ArrayCollection ) );
}

// -----------------------------------------------------

void CPSSPU_beginOfNewFunctionAssignName( void ) 
{
  assert( Text->size() >= 1 );

  const configParserStruct::command &LastCommand = (*Text)[ Text->size() - 1 ];
  if ( dynamic_cast< const configParserStruct::pushLocalDataRefCommand* >( &LastCommand ) )
  {
    const configParserStruct::reference Reference = LastCommand.argument().ref();
    Text->replaceLastCommand( configParserStruct::pushGlobalDataRefCommand( Reference.asLocalName() ) );
  }

  Text->eraseFromVariablesSet( LastCommand.argument().ref().asGlobalName() );
  Text->parseTimeIncFunctionLevel();
    
  const size_t TextSize = Text->size();
  Text->push( configParserStruct::pushInstructionRefCommand(TextSize+4) );
  Text->push( configParserStruct::assignCommand() );
  Text->push( configParserStruct::popAndAssignLastCommand() );
  Text->pushPlaceholder();
}

// -----------------------------------------------------

void CPSSPU_endOfCurrentFunction( void ) 
{
  Text->push( configParserStruct::pushValueCommand() );
  Text->push( configParserStruct::returnCommand() );
  const size_t TextSize = Text->size();
  Text->replacePlaceholder( configParserStruct::jumpCommand(TextSize) );
  Text->parseTimeDecFunctionLevel();
}

// -----------------------------------------------------

void CPSSPU_returnFromCurrentFunction( void ) 
{
  Text->push( configParserStruct::returnCommand() );
}

// -----------------------------------------------------

void CPSSPU_endOfProgram( void )
{
  assert( Text != NULL );
  Text->push( configParserStruct::haltCommand() );
}

// -----------------------------------------------------

void CPSSPU_prepareToFunctionCall( void ) 
{
  Text->push( configParserStruct::pushStackSizeCommand() );
}

// -----------------------------------------------------

void CPSSPU_pushFunctionArgument( void ) {}

// -----------------------------------------------------

void CPSSPU_callFunctionWithArgsFromStack( const char *Name ) 
{
  assert( Name != NULL );
  assert( Text != NULL );

  const char *FixedName = Text->constStringRef(Name);
  Text->push( configParserStruct::callCommand(FixedName) );
}

// -----------------------------------------------------

void CPSSPU_pushVariableValueToStack( void ) 
{
  const configParserStruct::command &LastCommand = (*Text)[ Text->size() - 1 ];
  if ( dynamic_cast< const configParserStruct::pushLocalDataRefCommand* >( &LastCommand ) )
  {
    const configParserStruct::reference Reference = LastCommand.argument().ref();
    Text->push( configParserStruct::derefCommand( Reference ) );
  } else {
    Text->push( configParserStruct::duptopCommand() );
    Text->push( configParserStruct::derefCommand( configParserStruct::derefCommand::RefFromStack ) );
  }
}

// -----------------------------------------------------

void CPSSPU_pushVariableReferenceToStack( const char *Name ) 
{
  assert( Text != NULL );
  const char *FixedName = Text->constStringRef(Name);
  Text->addToVariablesSet( FixedName );
  Text->push( configParserStruct::pushLocalDataRefCommand( FixedName ) );
}

// -----------------------------------------------------

void CPSSPU_pushArgumentReferenceToStack( int Number )
{
  Text->push( configParserStruct::pushLocalDataRefCommand( Number ) );
}

// -----------------------------------------------------

void CPSSPU_pushVariableGlobalReferenceToStack( const char *Name ) 
{
  assert( Text != NULL && Name != NULL );

  const char *FixedName = Text->constStringRef(Name);
  Text->push( configParserStruct::pushGlobalDataRefCommand( FixedName ) );
  Text->addToVariablesSet( FixedName, true );
}

// -----------------------------------------------------

void CPSSPU_replaceReferenceToValueOnStack( void ) 
{
  assert( Text != NULL );

  //std::cerr << "replace command " << typeid(LastCommand).name() << " arg " << LastCommand.argument() << std::endl;
  //const configParserStruct::reference Reference = LastCommand.argument().ref();
  //Text->replaceLastCommand( configParserStruct::derefCommand( Reference ) );

  const configParserStruct::command &LastCommand = (*Text)[ Text->size() - 1 ];
  if ( dynamic_cast< const configParserStruct::pushLocalDataRefCommand* >( &LastCommand ) )
  {
    const configParserStruct::reference Reference = LastCommand.argument().ref();
    Text->replaceLastCommand( configParserStruct::derefCommand( Reference ) );
  } else {
    Text->push( configParserStruct::derefCommand( configParserStruct::derefCommand::RefFromStack ) );
  }
}

// -----------------------------------------------------

void CPSSPU_setDictKeyString( const char *Name ) 
{
  assert( Name != NULL && Text != NULL );
  Text->push( configParserStruct::pushValueCommand( Name ) );
  Text->push( configParserStruct::joinrefCommand( configParserStruct::joinrefCommand::Dict ) );
}

// -----------------------------------------------------

void CPSSPU_setArrayIndexFromTop( void ) 
{
  Text->push( configParserStruct::joinrefCommand( configParserStruct::joinrefCommand::Array ) );
}

// -----------------------------------------------------

void CPSSPU_setDictKeyFromTop( void )
{
  Text->push( configParserStruct::joinrefCommand( configParserStruct::joinrefCommand::Dict ) );
}

// -----------------------------------------------------

void CPSSPU_pushVariableSymbolicReferenceToStack( void ) {}

// -----------------------------------------------------

void CPSSPU_beginOfIfStatement( void ) 
{
  Text->pushPlaceholder();
}

// -----------------------------------------------------

void CPSSPU_beginOfElseStatement( void ) 
{
  const size_t TextSize = Text->size();
  Text->replacePlaceholder( configParserStruct::jumpIfFalseCommand(TextSize+1) );
  Text->pushPlaceholder();
}

// -----------------------------------------------------

void CPSSPU_endOfIfElseStatement( void ) 
{
  const size_t TextSize = Text->size();
  Text->replacePlaceholder( configParserStruct::jumpCommand(TextSize) );
}

// -----------------------------------------------------

void CPSSPU_endOfIfStatement( void ) 
{
  const size_t TextSize = Text->size();
  Text->replacePlaceholder( configParserStruct::jumpIfFalseCommand(TextSize) );
}

// -----------------------------------------------------

void CPSSPU_prepareForWhileStatement( void ) 
{
  Text->pushPlaceholder();
}

// -----------------------------------------------------

void CPSSPU_beginOfWhileStatement( void ) 
{
  Text->pushPlaceholder();
}

// -----------------------------------------------------

void CPSSPU_endOfWhileStatement( void ) 
{
  const size_t TextSize = Text->size();
  Text->replacePlaceholder( configParserStruct::jumpIfFalseCommand(TextSize+1) );

  size_t IndexOfPlaceholder = Text->indexOfPlaceholder();
  Text->replacePlaceholder( configParserStruct::nopCommand() );
  Text->push( configParserStruct::jumpCommand(IndexOfPlaceholder+1) );
}

// -----------------------------------------------------

void CPSSPU_assignVariableValueFromStack( void ) 
{
  assert( Text != NULL );
  Text->push( configParserStruct::assignCommand() );
}

// -----------------------------------------------------

void CPSSPU_setDictFieldFromStack( const char *Name ) 
{
  Text->push( configParserStruct::joinDictCommand(Name) );
}

// -----------------------------------------------------

void CPSSPU_setArrayElementFromStack( void ) 
{
  Text->push( configParserStruct::joinArrayCommand() );
}

// -----------------------------------------------------

void CPSSPU_operatorOnStackTop( const char *OperatorType ) 
{
  assert( OperatorType != NULL );
  assert( Text != NULL );

#define CASE_OF_PUSH_COMMAND( Operator, Command ) \
  do { if ( std::strcmp( Operator, OperatorType ) == 0 ) { Text->push( configParserStruct::Command() ); return; } } while (false)

  CASE_OF_PUSH_COMMAND( "+",  addCommand );
  CASE_OF_PUSH_COMMAND( "-",  subCommand );
  CASE_OF_PUSH_COMMAND( "*",  mulCommand );
  CASE_OF_PUSH_COMMAND( "/",  divCommand );
  CASE_OF_PUSH_COMMAND( "%",  modCommand );
  CASE_OF_PUSH_COMMAND( "U-", unminusCommand );
  
  CASE_OF_PUSH_COMMAND( "==", numEqCommand );
  CASE_OF_PUSH_COMMAND( "!=", numNeCommand );
  CASE_OF_PUSH_COMMAND( "<",  numLtCommand );
  CASE_OF_PUSH_COMMAND( "<=", numLeCommand );
  CASE_OF_PUSH_COMMAND( ">",  numGtCommand );
  CASE_OF_PUSH_COMMAND( ">=", numGeCommand );
  
  CASE_OF_PUSH_COMMAND( "&&",  boolAndCommand );
  CASE_OF_PUSH_COMMAND( "||",  boolOrCommand  );
  CASE_OF_PUSH_COMMAND( "!",   boolNotCommand );
  
  CASE_OF_PUSH_COMMAND( ".==.", strEqCommand );
  CASE_OF_PUSH_COMMAND( ".!=.", strNeCommand );
  CASE_OF_PUSH_COMMAND( ".<.",  strLtCommand );
  CASE_OF_PUSH_COMMAND( ".<=.", strLeCommand );
  CASE_OF_PUSH_COMMAND( ".>.",  strGtCommand );
  CASE_OF_PUSH_COMMAND( ".>=.", strGeCommand );
  CASE_OF_PUSH_COMMAND( ".+.",  strAddCommand );

  assert( false );

#undef PUSH_COMMAND

}

// -----------------------------------------------------

void CPSSPU_finalizeExpressionStack( void ) 
{
  assert( Text != NULL );

  Text->push( configParserStruct::popAndAssignLastCommand() );
}

// -----------------------------------------------------

void CPSSPU_setStructParserError( void ) 
{
  assert( Text != NULL );

  int LineNumber = CPSSPU_lexCurrentLineNumber();
  Text->setErrorLine( LineNumber );
}

// =====================================================

