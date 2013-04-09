
DIGIT			[0-9]
LETTER                  [a-zA-Z_]
HEXDIGIT                [a-fA-F0-9]
INTNUM                  ({DIGIT}+)|(0x{HEXDIGIT}+)
REALNUM                 {DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?
STRING                  (\"[^\"]*\")|('[^']*')
TIME                    {DIGIT}({DIGIT}?):{DIGIT}({DIGIT}?)(:{DIGIT}({DIGIT})?)?
DATE                    {DIGIT}({DIGIT}?)\.{DIGIT}({DIGIT}?)\.{DIGIT}({DIGIT}?)
ID                      {LETTER}({LETTER}|{DIGIT})*
ARGUMENT                \${DIGIT}+
DELIM                   [ \t\v\r\f]

%{

// ====================================================

#include "configParserStruct/structparserconst.h"
#include "configParserStruct/structparserinput.h"
#include "configParserStruct/structparser.tab.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

extern void     CPSSPU_lexResetLineNumber( void );
extern unsigned CPSSPU_lexCurrentLineNumber( void ); 
extern void     CPSSPU_lexSetNewLineEnabled( int Enabled );
static void     CPSSPU_lexUpdateLineNumber( void );

static int  CPSSPU_lexEnableTokenNewline( void );
static void CPSSPU_lexProcessLexerPragma( const char *const String );

#ifdef YY_INPUT
#  undef  YY_INPUT
#endif

#define YY_INPUT( Buffer, ResultSize, MaxSize ) \
  do { ResultSize = getNextPartFromInputString(Buffer,MaxSize); } while ( 0 )

// ====================================================

%}

%s COMMENT LEXER_PRAGMA

%%

"#"              { BEGIN COMMENT;                          }
<COMMENT>"\n"    { CPSSPU_lexUpdateLineNumber(); BEGIN(INITIAL); if ( CPSSPU_lexEnableTokenNewline() ) return TOKEN_NEWLINE; }
<COMMENT>[^\n]*  {  } 

^[ \t]*!                { BEGIN LEXER_PRAGMA; }
<LEXER_PRAGMA>"\n"      { CPSSPU_lexUpdateLineNumber(); BEGIN INITIAL; }
<LEXER_PRAGMA>[^*\n]*   { CPSSPU_lexProcessLexerPragma(CPSSPU_text); }


{DELIM}         { CPSSPU_lexUpdateLineNumber(); }

";"             {  return ';';              }
"."             {  return '.';              }
","             {  return ',';              }

">="            {  CPSSPU_lval.CmpOpStr = ">="; return TOKEN_CMP; }
">"             {  CPSSPU_lval.CmpOpStr = ">";  return TOKEN_CMP; }
"<="            {  CPSSPU_lval.CmpOpStr = "<="; return TOKEN_CMP; }
"<"             {  CPSSPU_lval.CmpOpStr = "<";  return TOKEN_CMP; }
"=="            {  CPSSPU_lval.CmpOpStr = "=="; return TOKEN_CMP; }
"!="            {  CPSSPU_lval.CmpOpStr = "!="; return TOKEN_CMP; }

"+="            { return TOKEN_ADDEQ; }
"-="            { return TOKEN_SUBEQ; }
"*="            { return TOKEN_MULEQ; }
"/="            { return TOKEN_DIVEQ; }
"="             { return '=';               }

"("             {  return '(';              }
")"             {  return ')';              }

"+"             {  return '+';              }
"-"             {  return '-';              }
"**"            {  return TOKEN_POW;  }
"^"             {        }
"*"             {  return '*';              }
"/"             {  return '/';              }
"%"             {  return '%';              }

"&&"            {  return TOKEN_BOOL_AND;   }
"||"            {  return TOKEN_BOOL_OR;    }
"!"             {  return '!';   }

"?"             { return '?';               }
":"             { return ':';               }

"{"             { return '{';               }
"}"             { return '}';               }

"func"          { return TOKEN_FUNCTION;    }
"return"        { return TOKEN_RETURN;      }

"if"            { return TOKEN_IF;          }
"else"          { return TOKEN_ELSE;        }
"while"         { return TOKEN_WHILE;       }

{INTNUM}        {  
                   CPSSPU_lval.RealNumber = atol(CPSSPU_text); 
		   return TOKEN_REALNUMBER; 
		}

{REALNUM}       {  
                   CPSSPU_lval.RealNumber = atof(CPSSPU_text); 
		   return TOKEN_REALNUMBER; 
		}

{STRING}        {
                  CPSSPU_lexUpdateLineNumber();
                  if ( CPSSPU_leng > STRUCTPARSER_MAX_STRING_LENGTH-1 )
		  {
		    fprintf( stderr, "Error: max length of string is %i\n",STRUCTPARSER_MAX_STRING_LENGTH-1);
		    return TOKEN_ERROR;
		  }
                  CPSSPU_lval.String[STRUCTPARSER_MAX_STRING_LENGTH-1] = '\0';
                  strncpy(CPSSPU_lval.String,&CPSSPU_text[1],STRUCTPARSER_MAX_STRING_LENGTH-1); 
		  CPSSPU_lval.String[CPSSPU_leng-2] = '\0';  
		  return TOKEN_STRING;     
		}

{ID}            {
                  CPSSPU_lexUpdateLineNumber();
                  CPSSPU_lval.IdName[STRUCTPARSER_MAX_ID_LENGTH-1] = '\0';
                  strncpy( CPSSPU_lval.IdName, CPSSPU_text, STRUCTPARSER_MAX_ID_LENGTH-1 );
                  return TOKEN_ID;
                }

{ARGUMENT}      {
                  CPSSPU_lexUpdateLineNumber();
                  CPSSPU_lval.IdName[STRUCTPARSER_MAX_ID_LENGTH-1] = '\0';
                  strncpy( CPSSPU_lval.IdName, CPSSPU_text, STRUCTPARSER_MAX_ID_LENGTH-1 );
                  return TOKEN_ARGUMENT;
                }  


"\\\n"          { CPSSPU_lexUpdateLineNumber();                       }
"\n"            { CPSSPU_lexUpdateLineNumber(); if ( CPSSPU_lexEnableTokenNewline() ) return TOKEN_NEWLINE; }

.               { CPSSPU_lexUpdateLineNumber(); return TOKEN_ERROR;   }

%%

// =====================================================

#define TRUE 1
#define FALSE 0

static unsigned CPSSPU_LineNumber = 0;
static int CPSSPU_EnableNewline = FALSE;

// -----------------------------------------------------

void CPSSPU_lexResetLineNumber( void )
{
  CPSSPU_LineNumber = 0;
}

// -----------------------------------------------------

unsigned CPSSPU_lexCurrentLineNumber( void )
{
  return CPSSPU_LineNumber;
}

// -----------------------------------------------------

void CPSSPU_lexSetNewLineEnabled( int Enabled )
{
  CPSSPU_EnableNewline = Enabled;
}

// -----------------------------------------------------

static void CPSSPU_lexUpdateLineNumber( void )
{
  unsigned i;
  for ( i = 0; CPSSPU_text[i] != '\0'; i++ )
  {
    if ( CPSSPU_text[i] == '\n' )
      CPSSPU_LineNumber++; 
  }
}

// -----------------------------------------------------

static void CPSSPU_lexProcessLexerPragma( const char *const String )
{
  if ( strcmp( String, "NL+" ) == 0 )
    CPSSPU_lexSetNewLineEnabled( TRUE );
  if ( strcmp( String, "NL-" ) == 0 )
    CPSSPU_lexSetNewLineEnabled( FALSE );
}

// -----------------------------------------------------

static int CPSSPU_lexEnableTokenNewline( void )
{
  return CPSSPU_EnableNewline;
}

// =====================================================

