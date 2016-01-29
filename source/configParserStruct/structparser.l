
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
extern size_t   CPSSPU_lexCurrentLineNumber( void ); 
extern void     CPSSPU_lexSetNewLineEnabled( int Enabled );
static void     CPSSPU_lexUpdateLineNumber( void );

static int  CPSSPU_lexEnableTokenNewline( void );
static void CPSSPU_lexProcessLexerPragma( const char *const String );
static void CPSSPU_lexProcessAndCopyString( char *Destination, size_t DestinationSize, const char *const Source );

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

">="            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
">"             {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
"<="            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
"<"             {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
"=="            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
"!="            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }

".>=."            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
".>."             {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
".<=."            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
".<."             {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
".==."            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }
".!=."            {  strcpy( CPSSPU_lval.CmpOpStr, CPSSPU_text ); return TOKEN_CMP; }

"+="            { return TOKEN_ADDEQ; }
"-="            { return TOKEN_SUBEQ; }
"*="            { return TOKEN_MULEQ; }
"/="            { return TOKEN_DIVEQ; }
"="             { return '=';               }

".+=."          { return TOKEN_ADDEQ_STR; }

"("             {  return '(';              }
")"             {  return ')';              }

"+"             {  return '+';              }
"-"             {  return '-';              }
"*"             {  return '*';              }
"/"             {  return '/';              }
"%"             {  return '%';              }

".+."           {  return TOKEN_ADD_STR;    }

"&&"            {  return TOKEN_BOOL_AND;   }
"||"            {  return TOKEN_BOOL_OR;    }
"!"             {  return '!';   }

"?"             { return '?';               }
":"             { return ':';               }

"{"             { return '{';               }
"}"             { return '}';               }

"\["            { return '[';               }
"\]"            { return ']';               }

";"             {  return ';';              }
"."             {  return '.';              }
","             {  return ',';              }


"func"          { return TOKEN_FUNCTION;    }
"return"        { return TOKEN_RETURN;      }

"if"            { return TOKEN_IF;          }
"else"          { return TOKEN_ELSE;        }
"while"         { return TOKEN_WHILE;       }

"\${"           { return TOMEN_SYMREF_BEGIN; }

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
                  CPSSPU_lexProcessAndCopyString( CPSSPU_lval.String, STRUCTPARSER_MAX_STRING_LENGTH, CPSSPU_text );
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

static size_t CPSSPU_LineNumber = 0;
static int CPSSPU_EnableNewline = FALSE;

// -----------------------------------------------------

void CPSSPU_lexResetLineNumber( void )
{
  CPSSPU_LineNumber = 0;
}

// -----------------------------------------------------

size_t CPSSPU_lexCurrentLineNumber( void )
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
  size_t i;
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

// -----------------------------------------------------

static void CPSSPU_lexProcessAndCopyString( char *Destination, size_t DestinationSize, const char *const Source )
{
  size_t si, di;
  size_t SourceSize = 0;

  assert( Source != NULL );
  assert( Destination != NULL );
  assert( DestinationSize > 0 );

  Destination[0] = Destination[ DestinationSize-1 ] = '\0';

  SourceSize = strlen( Source );
  if ( SourceSize <= 2 )
    return;

  si = 1;
  di = 0;
  while ( si < SourceSize-1 && di < DestinationSize-1 )
  {
    if ( Source[si] == '\\' && si != SourceSize-2 )
    {
      si += 1;
      switch ( Source[si] )
      {
        case 'n':
          Destination[di] = '\n';
          break;

        case 't':
          Destination[di] = '\t';
          break;

        case 'r':
          Destination[di] = '\r';
          break;

        case '0':
        case '1':
        case '2':
          {
            char NumberStr[4];
            NumberStr[ 0 ] = ( si < SourceSize-1 ? Source[si++] : '\0' );
            NumberStr[ 1 ] = ( si < SourceSize-1 ? Source[si++] : '\0' );
            NumberStr[ 2 ] = ( si < SourceSize-1 ? Source[si]   : '\0' );
            NumberStr[ 3 ] = '\0';
            Destination[di] = (char)strtol( NumberStr, NULL, 8 );
            break;
          }

        case 'x':
          {
            char NumberStr[3];
            si++;
            NumberStr[ 0 ] = ( si < SourceSize-1 ? Source[si++] : '\0' );
            NumberStr[ 1 ] = ( si < SourceSize-1 ? Source[si]   : '\0' );
            NumberStr[ 2 ] = '\0';
            Destination[di] = (char)strtol( NumberStr, NULL, 16 );
            break;
          }

        default:
          Destination[di] = Source[si];
          break;
      }
    } else {
      Destination[di] = Source[si];
    }
    si += 1;
    di += 1;
  }

  assert( di < DestinationSize );
  Destination[di] = '\0';
}

// =====================================================

