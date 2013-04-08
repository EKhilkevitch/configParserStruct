
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

EXTERN int CPSSPU_parse( void );
EXTERN int CPSSPU_lex( void );
EXTERN void CPSSPU_error( const char *String );

// ====================================================

%}

%union 
{
  char String[STRUCTPARSER_MAX_STRING_LENGTH];
  char IdName[STRUCTPARSER_MAX_ID_LENGTH];
  int  IntNumber;
  double RealNumber;
  const char *CmpOpStr;
}

%token <String>     TOKEN_STRING
%token <IntNumber>  TOKEN_INTNUMBER
%token <RealNumber> TOKEN_REALNUMBER
%token <TimeVal>    TOKEN_TIMEVAL
%token <DateVal>    TOKEN_DATEVAL
%token <IdName>     TOKEN_ID
%token <IdName>     TOKEN_ARGUMENT
%token <CmpOpStr>   TOKEN_CMP

%token TOKEN_ERROR
%token TOKEN_NEWLINE
%token TOKEN_POW
%token TOKEN_ADDEQ
%token TOKEN_SUBEQ
%token TOKEN_MULEQ
%token TOKEN_DIVEQ
%token TOKEN_FUNCTION
%token TOKEN_RETURN
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_WHILE

%type  <IdName>    fullId

%right '='
%right '?' ':'

%%

program        : statementSeq 
	       | error { yyclearin; yyerrok; CPSSPU_setStructParserError(); }
	       ;

delimiter      : ';' 
               | TOKEN_NEWLINE
               ;

statementSeq   : statementSeq statement
               | 
               ;

block          : '{' statementSeq '}'
               ;

statement      : expression delimiter { CPSSPU_finalizeExpressionStack(); } 
	       | return
               | function
               | block
               | ifStatement 
               | delimiter
	       ;

return         : TOKEN_RETURN expression delimiter { CPSSPU_returnFromCurrentFunction(); }
               ;

function       : fullId '=' TOKEN_FUNCTION { CPSSPU_beginOfNewFunctionAssignName($1); } '{' program '}' { CPSSPU_endOfCurrentFunction(); }
               ;

ifStatement    : TOKEN_IF '(' exprSet ')' { CPSSPU_beginOfIfStatement(); } block elseStatement
               ;

elseStatement  : TOKEN_ELSE { CPSSPU_beginOfElseStatement(); } block { CPSSPU_endOfIfElseStatement(); }
               | { CPSSPU_endOfIfStatement(); }
               ;

whileStatement : TOKEN_WHILE '(' exprSet ')' { CPSSPU_beginOfWhileStatement(); } block { CPSSPU_endOfWhileStatement(); }
               ;

expression     : exprSet    {  }
               | fullId '=' { CPSSPU_pushDictToStack(); } '{' structFields '}' { CPSSPU_assignVariableValueFromStack($1);  }
	       ;

exprSet        : fullId '=' exprSet { CPSSPU_assignVariableValueFromStack($1); }
               | fullId { CPSSPU_pushVariableValueToStack($1); } TOKEN_ADDEQ exprSet { CPSSPU_operatorOnStackTop("+"); CPSSPU_assignVariableValueFromStack($1); }
               | fullId { CPSSPU_pushVariableValueToStack($1); } TOKEN_SUBEQ exprSet { CPSSPU_operatorOnStackTop("-"); CPSSPU_assignVariableValueFromStack($1); }
               | fullId { CPSSPU_pushVariableValueToStack($1); } TOKEN_MULEQ exprSet { CPSSPU_operatorOnStackTop("*"); CPSSPU_assignVariableValueFromStack($1); }
               | fullId { CPSSPU_pushVariableValueToStack($1); } TOKEN_DIVEQ exprSet { CPSSPU_operatorOnStackTop("/"); CPSSPU_assignVariableValueFromStack($1); }
               | exprThr {  }
               ;

exprThr        : exprCmp  { CPSSPU_beginOfIfStatement(); } 
                     '?' exprCmp { CPSSPU_beginOfElseStatement(); }
                     ':' exprThr { CPSSPU_endOfIfElseStatement(); }
               | exprCmp  {  }
               ;

exprCmp        : exprCmp TOKEN_CMP exprAdd { CPSSPU_operatorOnStackTop( $2 ); }
               | exprAdd                   {  }  
               ;

exprAdd        : exprAdd '+' exprMul  { CPSSPU_operatorOnStackTop("+"); }
	       | exprAdd '-' exprMul  { CPSSPU_operatorOnStackTop("-"); }
	       | exprMul              {  }
	       ;

exprMul        : exprMul '*' exprSign    { CPSSPU_operatorOnStackTop("*"); }
	       | exprMul '/' exprSign    { CPSSPU_operatorOnStackTop("/"); }
	       | exprSign                {  }
	       ;

exprSign       : exprAtom              {  }
	       | '-' { CPSSPU_pushRealNumberToStack(0); } exprAtom { CPSSPU_operatorOnStackTop("-"); }
	       | '+' exprAtom          {  }
	       ;

exprAtom       : fullId             { CPSSPU_pushVariableValueToStack( $1 ); } 
	       | TOKEN_REALNUMBER   { CPSSPU_pushRealNumberToStack( $1 ); } 
	       | TOKEN_INTNUMBER    { CPSSPU_pushIntegerNumberToStack( $1 ); }
               | TOKEN_STRING       { CPSSPU_pushStringToStack( $1 ); }
               | TOKEN_ID { CPSSPU_prepareToFunctionCall(); } '(' arglist ')' { CPSSPU_callFunctionWithArgsFromStack($1); }
                                    {  }
	       | '(' exprSet ')'    {  }
	       ;

arglist        : arglistx
               |                               {  } 
               ;

arglistx       : arglistx ',' expression       { CPSSPU_pushFunctionArgument();  }
               | expression                    { CPSSPU_pushFunctionArgument(); }
               ;

structFields   : structFields ',' structField
               | structField
               |
               ;

structField    : '.' TOKEN_ID '=' exprSet { CPSSPU_setDictFieldFromStack($2); }
               | '.' TOKEN_ID '=' { CPSSPU_pushDictToStack(); } '{' structFields '}' { CPSSPU_setDictFieldFromStack($2); }
               ;

fullId         : fullId '.' TOKEN_ID     { strncpy( $$, $1, STRUCTPARSER_MAX_ID_LENGTH/2-1 ); strncat( $$, ".", 2 ); strncat( $$, $3, STRUCTPARSER_MAX_ID_LENGTH/2-1 ); }
               | TOKEN_ID                { strncpy( $$, $1, STRUCTPARSER_MAX_ID_LENGTH ); }
               | TOKEN_ARGUMENT          { strncpy( $$, $1, STRUCTPARSER_MAX_ID_LENGTH ); }
               ;

%%

// ==================================================

int CPSSPU_wrap( void )
{
  return 1;
  CPSSPU_FLUSH_BUFFER(); 
}

// --------------------------------------------------

void CPSSPU_error( const char *String )
{
  CPSSPU_setStructParserError();
}

// ====================================================

