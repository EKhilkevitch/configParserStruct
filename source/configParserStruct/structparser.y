
%{

// ====================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "configParserStruct/structparserutil.h"

// ====================================================

#if __cplusplus
#  define EXTERN extern "C" 
#else
#  define EXTERN extern
#endif

EXTERN int parserwrap( void );
EXTERN int yyparse( void );
EXTERN int yylex( void );
EXTERN void yyerror( const char *const String );

EXTERN unsigned lexCurrentLineNumber( void );

#if 0
EXTERN void yyparserSetParsedProg( struct parserProgram *const Program );
EXTERN struct parserProgram* yyparserProgram( void );

static unsigned setOperationInProgram( const enum parserOperation Operation, const char* const SetVariable, const unsigned ExprReault,
  struct parserProgram *const Program );

static enum parserOperation convertCmpTypeToOperation( const enum parserCmpType CmpType );
#endif

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
	       | error { yyerrok; yyclearin; yyerror("invalid parserCommand"); }
	       ;

delimiter      : ';' 
               | TOKEN_NEWLINE
               ;


parserCommand  : expression {  }
	       |
	       ;

expression     : exprSet    { $$ = $1; }
               | fullId '=' {  } '{' structFields '}' {  }
	       ;

exprSet        : fullId '=' exprSet  {  }
               | fullId TOKEN_ADDEQ exprSet {   }
               | fullId TOKEN_MULEQ exprSet {   }
               | fullId TOKEN_SUBEQ exprSet {   }
               | fullId TOKEN_DIVEQ exprSet {   }
               | exprThr             {  }
               ;


exprThr        : exprCmp  {    } 
                     '?' exprCmp {  }
                     ':' exprThr {  }
               | exprCmp  {  }
               ;

exprCmp        : exprCmp TOKEN_CMP exprAdd { }
               | exprAdd              {  }  


exprAdd        : exprAdd '+' exprMul  {  }
	       | exprAdd '-' exprMul  {  }
	       | exprMul              {  }
	       ;

exprMul        : exprMul '*' exprSign    {  }
	       | exprMul '/' exprSign    {  }
	       | exprSign                {  }
	       ;

exprSign       : exprAtom              {  }
	       | '-' exprAtom          {  }
	       | '+' exprAtom          { }
	       ;

exprAtom       : fullId             {  } 
	       | TOKEN_REALNUMBER   {  } 
	       | TOKEN_INTNUMBER    {  }
               | TOKEN_STRING       {  }
               | TOKEN_ID {  } '(' arglist ')' 
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

structField    : '.' TOKEN_ID '=' exprSet {  }
               | '.' TOKEN_ID '=' {  } '{' structFields '}' 
                                  {  }
               ;

fullId         : fullId '.' TOKEN_ID     {  }
               | TOKEN_ID                {  }
               ;

%%

// ==================================================

void yyerror( const char *const String )
{
#if 0
  if ( yyparserProgram()->NumberOfErrorLine < 0 )
  {
    yyparserProgram()->NumberOfErrorLine = lexCurrentLineNumber();
#if ! SILENT_ERRORS
    fprintf(stderr,"Error: %s, line %u\n", String, lexCurrentLineNumber() ); 
#endif
  }
#endif
}

// ==================================================

int yywrap( void )
{
  return 1;
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




