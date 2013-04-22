
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
%token TOKEN_BOOL_AND
%token TOKEN_BOOL_OR
%token TOKEN_FUNCTION
%token TOKEN_RETURN
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_WHILE

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

statement      : delimiter
               | function
               | expression delimiter { CPSSPU_finalizeExpressionStack(); }
	       | return
               | block
               | ifStatement 
               | whileStatement
	       ;

return         : TOKEN_RETURN expression delimiter { CPSSPU_returnFromCurrentFunction(); }
               ;

function       : idRef '=' TOKEN_FUNCTION { CPSSPU_beginOfNewFunctionAssignName(); } '{' program '}' { CPSSPU_endOfCurrentFunction(); }
               ;

ifStatement    : TOKEN_IF '(' exprSet ')' { CPSSPU_beginOfIfStatement(); } block elseStatement
               ;

elseStatement  : TOKEN_ELSE { CPSSPU_beginOfElseStatement(); } block { CPSSPU_endOfIfElseStatement(); }
               | { CPSSPU_endOfIfStatement(); }
               ;

whileStatement : TOKEN_WHILE { CPSSPU_prepareForWhileStatement(); } '(' exprSet ')' { CPSSPU_beginOfWhileStatement(); } block { CPSSPU_endOfWhileStatement(); }
               ;

expression     : exprSet    
               | idRef '=' { CPSSPU_pushDictToStack();  } '{' structFields '}' { CPSSPU_assignVariableValueFromStack(); }
               | idRef '=' { CPSSPU_pushArrayToStack(); } '[' arrayElements ']'{ CPSSPU_assignVariableValueFromStack(); }
	       ;

exprSet        : idRef '=' exprSet { CPSSPU_assignVariableValueFromStack(); }
               | idRef { CPSSPU_pushVariableValueToStack(); } TOKEN_ADDEQ exprSet { CPSSPU_operatorOnStackTop("+"); CPSSPU_assignVariableValueFromStack(); }
               | idRef { CPSSPU_pushVariableValueToStack(); } TOKEN_SUBEQ exprSet { CPSSPU_operatorOnStackTop("-"); CPSSPU_assignVariableValueFromStack(); }
               | idRef { CPSSPU_pushVariableValueToStack(); } TOKEN_MULEQ exprSet { CPSSPU_operatorOnStackTop("*"); CPSSPU_assignVariableValueFromStack(); }
               | idRef { CPSSPU_pushVariableValueToStack(); } TOKEN_DIVEQ exprSet { CPSSPU_operatorOnStackTop("/"); CPSSPU_assignVariableValueFromStack(); }
               | exprThr 
               ;

exprThr        : exprBool  { CPSSPU_beginOfIfStatement(); } 
                     '?' exprBool { CPSSPU_beginOfElseStatement(); }
                     ':' exprThr  { CPSSPU_endOfIfElseStatement(); }
               | exprBool
               ;

exprBool       : exprBool TOKEN_BOOL_AND exprCmp { CPSSPU_operatorOnStackTop("&&"); }
               | exprBool TOKEN_BOOL_OR  exprCmp { CPSSPU_operatorOnStackTop("||"); }
               | exprCmp
               ;

exprCmp        : exprCmp TOKEN_CMP exprAdd { CPSSPU_operatorOnStackTop( $2 ); }
               | exprAdd                   
               ;

exprAdd        : exprAdd '+' exprMul  { CPSSPU_operatorOnStackTop("+"); }
	       | exprAdd '-' exprMul  { CPSSPU_operatorOnStackTop("-"); }
	       | exprMul              
	       ;

exprMul        : exprMul '*' exprSign    { CPSSPU_operatorOnStackTop("*"); }
	       | exprMul '/' exprSign    { CPSSPU_operatorOnStackTop("/"); }
               | exprMul '%' exprSign    { CPSSPU_operatorOnStackTop("%"); }
	       | exprSign                
	       ;

exprSign       : exprAtom
	       | '-' { CPSSPU_pushRealNumberToStack(0); } exprAtom { CPSSPU_operatorOnStackTop("-"); }
	       | '+' exprAtom          
               | '!' exprSign          { CPSSPU_operatorOnStackTop("!"); }
	       ;

exprAtom       : idRef             { CPSSPU_replaceReferenceToValueOnStack(); } 
	       | TOKEN_REALNUMBER   { CPSSPU_pushRealNumberToStack($1); } 
	       | TOKEN_INTNUMBER    { CPSSPU_pushIntegerNumberToStack( $1 ); }
               | TOKEN_STRING       { CPSSPU_pushStringToStack( $1 ); }
               | functionCall
	       | '(' exprSet ')'
	       ;

functionCall   : TOKEN_ID           { CPSSPU_prepareToFunctionCall(); } '(' arguments ')' { CPSSPU_callFunctionWithArgsFromStack($1); }
               ;

arguments      : argumentsList
               |        
               ;

argumentsList  : argumentsList ',' expression  { CPSSPU_pushFunctionArgument();  }
               | expression                    { CPSSPU_pushFunctionArgument(); }
               ;

arrayElements  : arrayElementsList
               |
               ;

arrayElementsList : arrayElementsList ',' expression { CPSSPU_setArrayElementFromStack(); }
               | expression                          { CPSSPU_setArrayElementFromStack(); }
               ;

structFields   : structFields ',' structField
               | structField
               |
               ;

structField    : '.' TOKEN_ID '=' exprSet { CPSSPU_setDictFieldFromStack($2); }
               | '.' TOKEN_ID '=' { CPSSPU_pushDictToStack(); } '{' structFields '}' { CPSSPU_setDictFieldFromStack($2); }
               ;

idRef          : idRef '.' TOKEN_ID      { CPSSPU_setAttributeToTopReference( $3 );   }
               | TOKEN_ID                { CPSSPU_pushVariableReferenceToStack( $1 ); }
               | TOKEN_ARGUMENT          { CPSSPU_pushVariableReferenceToStack( $1 ); }
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

