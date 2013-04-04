
// =====================================================

#include "configParserStruct/structparserinput.h"

#include <string>
#include <cstring>
#include <cstdlib>

// =====================================================

static configParserStruct::structParser::inputString ParserString;

// =====================================================

void structParserSetInputString( const char *String )
{
  ParserString.set( String );
}

// -----------------------------------------------------

size_t getNextPartFromInputString( char *const Buffer, const size_t MaxSize )
{
  return ParserString.putNextPartToBuffer( Buffer, MaxSize );
}

// =====================================================
      
configParserStruct::structParser::inputString::inputString( const char *S ) : 
  Position(0) 
{ 
  set(S); 
}

// -----------------------------------------------------

void configParserStruct::structParser::inputString::set( const char *String )
{
  this->String = ( String == NULL ) ? "" : String;
  this->Position = 0;
}

// -----------------------------------------------------

const std::string configParserStruct::structParser::inputString::takeNextPart( size_t Size )
{
  if ( Position >= String.length() )
    return std::string();
  std::string Result = String.substr( Position, Size );
  Position += Size;
  return Result;
}

// -----------------------------------------------------

size_t configParserStruct::structParser::inputString::putNextPartToBuffer( char *Buffer, size_t Size )
{
  if ( Buffer == NULL || Size <= 0 )
    return 0;
  
  Buffer[ Size - 1 ] = Buffer[ 0 ] = '\0';

  std::string NextPart = takeNextPart(Size);
  std::strncpy( Buffer, NextPart.c_str(), Size );
  return NextPart.length();
}

// =====================================================


