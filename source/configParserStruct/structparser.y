
%{

// ====================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "configParserStruct/structparserconst.h"
#include "configParserStruct/structparsercompiler.h"

// ====================================================

#if __cplusplus
#  define EXTERN extern "C" 
#else
#  define EXTERN extern
#endif

EXTERN int strprs_parse( void );
EXTERN int strprs_lex( void );
EXTERN void strprs_error( const char *String );

// ====================================================

%}

%union 
{
  char String[STRUCTPARSER_MAX_STRING_LENGTH];
  char IdName[STRUCTPARSER_MAX_ID_LENGTH];
  int  IntNumber;
  double RealNumber;
  int CmpType;
  unsigned ExprValue;
}

%token <String>     TOKEN_STRING
%token <IntNumber>  TOKEN_INTNUMBER
%token <RealNumber> TOKEN_REALNUMBER
%token <TimeVal>    TOKEN_TIMEVAL
%token <DateVal>    TOKEN_DATEVAL
%token <IdName>     TOKEN_ID
%token <CmpType>    TOKEN_CMP
%token TOKEN_ERROR
%token TOKEN_NEWLINE
%token TOKEN_POW
%token TOKEN_ADDEQ
%token TOKEN_SUBEQ
%token TOKEN_MULEQ
%token TOKEN_DIVEQ

%type  <ExprValue> exprSet
%type  <ExprValue> exprThr
%type  <ExprValue> exprCmp
%type  <ExprValue> exprAdd
%type  <ExprValue> exprMul
%type  <ExprValue> exprSign
%type  <ExprValue> exprAtom
%type  <ExprValue> expression
%type  <IdName>    fullId

%right '='

%%

parserCommands : parserCommands parserCommand delimiter
	       | parserCommand delimiter
	       | error { yyclearin; yyerrok; setStructParserError(); }
	       ;

delimiter      : ';' 
               | TOKEN_NEWLINE
               ;

parserCommand  : expression { finalizeExpressionStack(); }
	       |
	       ;

expression     : exprSet    {  }
               | fullId '=' { pushDictToStack(); } '{' structFields '}' { assignVariableValueFromStack($1);  }
	       ;

exprSet        : fullId '=' exprSet { assignVariableValueFromStack($1); }
               | fullId { pushVariableValueToStack($1); } TOKEN_ADDEQ exprSet { operatorOnStackTop('+'); assignVariableValueFromStack($1); }
               | fullId { pushVariableValueToStack($1); } TOKEN_SUBEQ exprSet { operatorOnStackTop('-'); assignVariableValueFromStack($1); }
               | fullId { pushVariableValueToStack($1); } TOKEN_MULEQ exprSet { operatorOnStackTop('*'); assignVariableValueFromStack($1); }
               | fullId { pushVariableValueToStack($1); } TOKEN_DIVEQ exprSet { operatorOnStackTop('/'); assignVariableValueFromStack($1); }
               | exprThr {  }
               ;

exprThr        : exprCmp  {    } 
                     '?' exprCmp {  }
                     ':' exprThr {  }
               | exprCmp  {  }
               ;

exprCmp        : exprCmp TOKEN_CMP exprAdd { }
               | exprAdd              {  }  


exprAdd        : exprAdd '+' exprMul  { operatorOnStackTop('+'); }
	       | exprAdd '-' exprMul  { operatorOnStackTop('-'); }
	       | exprMul              {  }
	       ;

exprMul        : exprMul '*' exprSign    { operatorOnStackTop('*'); }
	       | exprMul '/' exprSign    { operatorOnStackTop('/'); }
	       | exprSign                {  }
	       ;

exprSign       : exprAtom              {  }
	       | '-' { pushRealNumberToStack(0); } exprAtom { operatorOnStackTop('-'); }
	       | '+' exprAtom          {  }
	       ;

exprAtom       : fullId             { pushVariableValueToStack( $1 ); } 
	       | TOKEN_REALNUMBER   { pushRealNumberToStack( $1 ); } 
	       | TOKEN_INTNUMBER    { pushIntegerNumberToStack( $1 ); }
               | TOKEN_STRING       { pushStringToStack( $1 ); }
               | TOKEN_ID { /*pushFunctionRetAddressToStack();*/ } '(' arglist ')' { /*callFunctionWithArgsFromStack($1);*/ }
                                    {  }
	       | '(' exprSet ')'    {  }
	       ;

arglist        : arglistx
               |                               {  } 
               ;

arglistx       : arglistx ',' expression       {  }
               | expression                    {  }
               ;

structFields   : structFields ',' structField
               | structField
               |
               ;

structField    : '.' TOKEN_ID '=' exprSet { setDictFieldFromStack($2); }
               | '.' TOKEN_ID '=' { pushDictToStack(); } '{' structFields '}' { setDictFieldFromStack($2); }
               ;

fullId         : fullId '.' TOKEN_ID     { strncpy( $$, $1, STRUCTPARSER_MAX_ID_LENGTH/2-1 ); strncat( $$, ".", 2 ); strncat( $$, $3, STRUCTPARSER_MAX_ID_LENGTH/2-1 ); }
               | TOKEN_ID                { strncpy( $$, $1, STRUCTPARSER_MAX_ID_LENGTH ); }
               ;

%%

// ==================================================

int strprs_wrap( void )
{
  return 1;
  strprs_FLUSH_BUFFER(); 
}

// --------------------------------------------------

void strprs_error( const char *String )
{
 setStructParserError();
}

// ====================================================

#if 0
void yyparserSetParsedProg( struct parserProgram *const Program )
{
  PrsProgram = Program;
}

// --------------------------------------------------

struct parserProgram* yyparserProgram( void )
{
  return PrsProgram;
}

// ====================================================

static unsigned setOperationInProgram( const enum parserOperation Operation, const char *const SetVariable, const unsigned ExprReault,
  struct parserProgram *const Program )
{
  unsigned Id    = parserIntrnlAddVariableToProgram( SetVariable, NULL, Program ); 
  unsigned tmpId = parserIntrnlAddCommandToProgram( Operation, Id, ExprReault, Program );
  return parserIntrnlAddCommandToProgram( PARSER_OPERATION_SET, Id, tmpId, Program );   
}

// --------------------------------------------------

static enum parserOperation convertCmpTypeToOperation( const enum parserCmpType CmpType )
{
  switch ( CmpType )
  {
    case PARSER_CMP_OPERATOR_GE:   return PARSER_OPERATION_GE;
    case PARSER_CMP_OPERATOR_GT:   return PARSER_OPERATION_GT;
    case PARSER_CMP_OPERATOR_LT:   return PARSER_OPERATION_LT;
    case PARSER_CMP_OPERATOR_LE:   return PARSER_OPERATION_LE;
    case PARSER_CMP_OPERATOR_EQ:   return PARSER_OPERATION_EQ;
    case PARSER_CMP_OPERATOR_NE:   return PARSER_OPERATION_NE;
    default:    
      abort();
      return PARSER_OPERATION_NOP;
  }
}
#endif
// ====================================================




