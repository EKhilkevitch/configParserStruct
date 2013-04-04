
DIGIT			[0-9]
LETTER                  [a-zA-Z_]
HEXDIGIT                [a-fA-F0-9]
INTNUM                  ({DIGIT}+)|(0x{HEXDIGIT}+)
REALNUM                 {DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?
STRING                  (\"[^\"]*\")|('[^']*')
TIME                    {DIGIT}({DIGIT}?):{DIGIT}({DIGIT}?)(:{DIGIT}({DIGIT})?)?
DATE                    {DIGIT}({DIGIT}?)\.{DIGIT}({DIGIT}?)\.{DIGIT}({DIGIT}?)
ID                      {LETTER}({LETTER}|{DIGIT})*
DELIM                   [ \t\v\r\f]

%{

// ====================================================

#include "configParserStruct/structparserutil.h"
#include "configParserStruct/structparser.tab.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#if LINUX
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

static void     lexResetLineNumber( void );
static unsigned lexCurrentLineNumber( void ); 
static void     lextUpdateLineNumber( void );

static int  lexEnableTokenNewline( void );
static void lexProcessLexerPragma( const char *const String );

#undef  YY_INPUT
#define YY_INPUT( Buffer, ResultSize, MaxSize ) \
  do { ResultSize = getNextPartFromInputString(Buffer,MaxSize); } while ( 0 )

// ====================================================

%}

%s COMMENT LEXER_PRAGMA

%%

"#"              { BEGIN COMMENT;                          }
<COMMENT>"\n"    { lextUpdateLineNumber(); BEGIN(INITIAL); if ( lexEnableTokenNewline() ) return TOKEN_NEWLINE; }
<COMMENT>[^\n]*  {  } 

^[ \t]*!                { BEGIN LEXER_PRAGMA; }
<LEXER_PRAGMA>"\n"      { lextUpdateLineNumber(); BEGIN INITIAL; }
<LEXER_PRAGMA>[^*\n]*   { lexProcessLexerPragma(yytext); }


{DELIM}         { lextUpdateLineNumber(); /* DISABLE_UNUSUAL_FUNC_WARNING( yyunput ); DISABLE_UNUSUAL_FUNC_WARNING( input ); */ }

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

"+="            {    }
"-="            {    }
"*="            {    }
"/="            {    }
"="             { return '=';               }

"("             {  return '(';              }
")"             {  return ')';              }

"+"             {  return '+';              }
"-"             {  return '-';              }
"**"            {        }
"^"             {        }
"*"             {  return '*';              }
"/"             {  return '/';              }
"%"             {  return '%';              }

"?"             { return '?';               }
":"             { return ':';               }

"{"             { return '{';               }
"}"             { return '}';               }

{INTNUM}        {  
                   yylval.RealNumber = atol(yytext); 
		   return TOKEN_REALNUMBER; 
		}

{REALNUM}       {  
                   yylval.RealNumber = atof(yytext); 
		   return TOKEN_REALNUMBER; 
		}

{STRING}        {
                  lextUpdateLineNumber();
                  if ( yyleng > STRUCTPARSER_MAX_STRING_LENGTH-1 )
		  {
		    fprintf( stderr, "Error: max length of string is %i\n",STRUCTPARSER_MAX_STRING_LENGTH-1);
		    return TOKEN_ERROR;
		  }
                  yylval.String[STRUCTPARSER_MAX_STRING_LENGTH-1] = '\0';
                  strncpy(yylval.String,&yytext[1],STRUCTPARSER_MAX_STRING_LENGTH-1); 
		  yylval.String[yyleng-2] = '\0';  
		  return TOKEN_STRING;     
		}

{ID}            {
                  lextUpdateLineNumber();
                  yylval.IdName[STRUCTPARSER_MAX_ID_LENGTH-1] = '\0';
                  strncpy( yylval.IdName, yytext, STRUCTPARSER_MAX_ID_LENGTH-1 );
                  return TOKEN_ID;
                }


"\\\n"          { lextUpdateLineNumber();                       }
"\n"            { lextUpdateLineNumber(); if ( lexEnableTokenNewline() ) return TOKEN_NEWLINE; }

.               { lextUpdateLineNumber(); return TOKEN_ERROR;   }

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

static void lextUpdateLineNumber( void )
{
  unsigned i;
  for ( i = 0; yytext[i] != '\0'; i++ )
  {
    if ( yytext[i] == '\n' )
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

