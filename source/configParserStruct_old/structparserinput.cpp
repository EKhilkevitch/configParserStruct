
// =====================================================

#include "configParserStruct/structparserinput.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

// =====================================================

static configParserStruct::structParserUtil::inputString ParserString;

// =====================================================

void configParserStruct::structParserUtil::setInputString( const std::string &String )
{
  ParserString.set( String );
}

// -----------------------------------------------------

size_t getNextPartFromInputString( char *const Buffer, const size_t MaxSize )
{
  return ParserString.putNextPartToBuffer( Buffer, MaxSize );
}

// =====================================================
      
configParserStruct::structParserUtil::inputString::inputString( const std::string &S ) : 
  Position(0) 
{ 
  set(S); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::inputString::set( const std::string &String )
{
  this->String = String;
  this->Position = 0;
}

// -----------------------------------------------------

const std::string configParserStruct::structParserUtil::inputString::takeNextPart( size_t Size )
{
  if ( Position >= String.length() )
    return std::string();
  std::string Result = String.substr( Position, Size );
  Position += Size;
  return Result;
}

// -----------------------------------------------------

size_t configParserStruct::structParserUtil::inputString::putNextPartToBuffer( char *Buffer, size_t Size )
{
  if ( Buffer == NULL || Size <= 0 )
    return 0;
 
  std::memset( Buffer, 'Z', Size );
  Buffer[ Size - 1 ] = Buffer[ 0 ] = '\0';

  std::string NextPart = takeNextPart(Size);
  std::strncpy( Buffer, NextPart.c_str(), Size );

//  std::printf( "putNextPartToBuffer: Size = %i, NextPart = %s, Buffer = %s\n", Size, NextPart.c_str(), Buffer );

  return NextPart.length();
}

// =====================================================


