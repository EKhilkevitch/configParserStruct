
/* ===================================================== */

#include "configParserStruct2/lexinput.h"

#include <string.h>
#include <stdlib.h>

/* ===================================================== */

struct parserInputString
{
  char *String;
  size_t Length;
  size_t Position;
};

static struct parserInputString ParserInputString = { NULL, 0, 0 };

/* ===================================================== */

int CPSSPU_setParserInputString( const char *String )
{
  char *StringCopy = NULL;
  size_t Length = 0;

  if ( String == NULL )
    return 0;

  Length = strlen( String );

  StringCopy = malloc( Length + 1 );
  if ( StringCopy == NULL )
    return 0;
  strcpy( StringCopy, String );

  free( ParserInputString.String );
  ParserInputString.String = StringCopy;
  ParserInputString.Length = Length;
  ParserInputString.Position = 0;

  return 1;
}

/* ----------------------------------------------------- */

void CPSSPU_clearParserInputString( void )
{
  free( ParserInputString.String );

  ParserInputString.String = NULL;
  ParserInputString.Length = 0;
  ParserInputString.Position = 0;
}

/* ----------------------------------------------------- */

size_t CPSSPU_takeNextFromParserInputString( char *Buffer, size_t MaxSize )
{
  size_t AvailableChars;

  if ( Buffer == NULL || MaxSize <= 0 )
    return 0;

  if ( ParserInputString.String == NULL )
  {
    Buffer[0] = '\0';
    return 0;
  }

  AvailableChars = ParserInputString.Length - ParserInputString.Position;

  strncpy( Buffer, ParserInputString.String + ParserInputString.Position, MaxSize - 1 );
  Buffer[ MaxSize - 1 ] = '\0';

  ParserInputString.Position += MaxSize - 1;
  if ( ParserInputString.Position > ParserInputString.Length )
    ParserInputString.Position = ParserInputString.Length;

  if ( AvailableChars >= MaxSize - 1 )
    return MaxSize - 1;
  else
    return AvailableChars;
}

/* ===================================================== */

