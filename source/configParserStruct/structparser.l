
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

extern void     lexResetLineNumber( void );
extern unsigned lexCurrentLineNumber( void ); 
static void     lexUpdateLineNumber( void );

static int  lexEnableTokenNewline( void );
static void lexProcessLexerPragma( const char *const String );

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
<COMMENT>"\n"    { lexUpdateLineNumber(); BEGIN(INITIAL); if ( lexEnableTokenNewline() ) return TOKEN_NEWLINE; }
<COMMENT>[^\n]*  {  } 

^[ \t]*!                { BEGIN LEXER_PRAGMA; }
<LEXER_PRAGMA>"\n"      { lexUpdateLineNumber(); BEGIN INITIAL; }
<LEXER_PRAGMA>[^*\n]*   { lexProcessLexerPragma(strprs_text); }


{DELIM}         { lexUpdateLineNumber(); }

";"             {  return ';';              }
"."             {  return '.';              }
","             {  return ',';              }

">="            {  }
">"             {  }
"<="            {  }
"<"             {  }
"=="            {  }
"<>"            {  }
"!="            {  }

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

"?"             { return '?';               }
":"             { return ':';               }

"{"             { return '{';               }
"}"             { return '}';               }

"func"          { return TOKEN_FUNCTION;    }
"return"        { return TOKEN_RETURN;      }

{INTNUM}        {  
                   strprs_lval.RealNumber = atol(strprs_text); 
		   return TOKEN_REALNUMBER; 
		}

{REALNUM}       {  
                   strprs_lval.RealNumber = atof(strprs_text); 
		   return TOKEN_REALNUMBER; 
		}

{STRING}        {
                  lexUpdateLineNumber();
                  if ( strprs_leng > STRUCTPARSER_MAX_STRING_LENGTH-1 )
		  {
		    fprintf( stderr, "Error: max length of string is %i\n",STRUCTPARSER_MAX_STRING_LENGTH-1);
		    return TOKEN_ERROR;
		  }
                  strprs_lval.String[STRUCTPARSER_MAX_STRING_LENGTH-1] = '\0';
                  strncpy(strprs_lval.String,&strprs_text[1],STRUCTPARSER_MAX_STRING_LENGTH-1); 
		  strprs_lval.String[strprs_leng-2] = '\0';  
		  return TOKEN_STRING;     
		}

{ID}            {
                  lexUpdateLineNumber();
                  strprs_lval.IdName[STRUCTPARSER_MAX_ID_LENGTH-1] = '\0';
                  strncpy( strprs_lval.IdName, strprs_text, STRUCTPARSER_MAX_ID_LENGTH-1 );
                  return TOKEN_ID;
                }

{ARGUMENT}      {
                  lexUpdateLineNumber();
                  strprs_lval.IdName[STRUCTPARSER_MAX_ID_LENGTH-1] = '\0';
                  strncpy( strprs_lval.IdName, strprs_text, STRUCTPARSER_MAX_ID_LENGTH-1 );
                  return TOKEN_ARGUMENT;
                }  


"\\\n"          { lexUpdateLineNumber();                       }
"\n"            { lexUpdateLineNumber(); if ( lexEnableTokenNewline() ) return TOKEN_NEWLINE; }

.               { lexUpdateLineNumber(); return TOKEN_ERROR;   }

%%

// =====================================================

#define TRUE 1
#define FALSE 0

static unsigned LineNumber = 0;
static int EnableNewline = FALSE;

// -----------------------------------------------------

void lexResetLineNumber( void )
{
  LineNumber = 0;
}

// -----------------------------------------------------

unsigned lexCurrentLineNumber( void )
{
  return LineNumber;
}

// -----------------------------------------------------

static void lexUpdateLineNumber( void )
{
  unsigned i;
  for ( i = 0; strprs_text[i] != '\0'; i++ )
  {
    if ( strprs_text[i] == '\n' )
      LineNumber++; 
  }
}

// -----------------------------------------------------

static void lexProcessLexerPragma( const char *const String )
{
  if ( strcmp( String, "NL+" ) == 0 )
    EnableNewline = TRUE;
  if ( strcmp( String, "NL-" ) == 0 )
    EnableNewline = FALSE;
}

// -----------------------------------------------------

static int  lexEnableTokenNewline( void )
{
  return EnableNewline;
}

// =====================================================

