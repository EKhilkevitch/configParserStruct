
// =====================================================

#include "configParserStruct/parser.h"
#include "configParserStruct/stringcast.h"

#include <cassert>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <iostream>

// =====================================================
      
std::list<std::string> configParserStruct::parser::readFileContent( const std::string &FileName )
{
  FILE *File = NULL;
  std::list<std::string> Result;

  if ( FileName == "-" )
  {
    File = stdin;
  } else {
    File = std::fopen( FileName.c_str(), "r" );
    if ( File == NULL )
      throw std::runtime_error("Can not open file '" + FileName + "': error is " + std::strerror(errno) );
  }

  char Buffer[8192];
  Buffer[ sizeof(Buffer)-1 ] = Buffer[0] = '\0';

  while ( true )
  {
    const char *RetValue = std::fgets( Buffer, sizeof(Buffer)-1, File );
    if ( RetValue == NULL )
      break;
    Result.push_back(Buffer);
  }

  if ( File != stdin )
    std::fclose(File);

  return Result;
}

// -----------------------------------------------------

std::string configParserStruct::parser::joinStringList( const std::list<std::string> &List, const std::string &JoinString )
{
  std::string Result;

  Result.reserve( ( JoinString.size() + 128 ) * List.size() );

  for ( std::list<std::string>::const_iterator i = List.begin(); i != List.end(); ++i )
  {
    Result += *i;
    Result += JoinString;
  }

  return Result;
}

// -----------------------------------------------------

std::string configParserStruct::parser::readFileContentinSingleString( const std::string &FileName, const std::string &JoinString )
{
  const std::list<std::string> &FileContent = readFileContent( FileName );
  const std::string &FileContentString = joinStringList( FileContent, JoinString );
  return FileContentString;
}

// -----------------------------------------------------
      
std::list<std::string> configParserStruct::parser::splitString( const std::string &String, const std::string &PossibleSeparators )
{
  std::list<std::string> Result;

  size_t Position = 0;
  while ( true )
  {
    size_t NextPosition = String.find_first_of( PossibleSeparators, Position );
    Result.push_back( String.substr( Position, NextPosition - Position ) );
    if ( NextPosition == std::string::npos )
      break;
    Position = NextPosition + 1; 
  }

  return Result;
}

// -----------------------------------------------------
      
void configParserStruct::parser::execFile( const std::string &FileName ) 
{ 
  const std::list<std::string> &FileContent = readFileContent( FileName );
  const std::string &FileContentString = joinStringList( FileContent, "" );
  exec( FileContentString ); 
}

// -----------------------------------------------------

void configParserStruct::parser::setVariableValueString( const std::string &VarName, const std::string &Value ) 
{
  assert( ! "You must implement this function OR reimplement setVariableValue() functions" );
}

// -----------------------------------------------------

void configParserStruct::parser::setVariableValue( const std::string &VarName, const std::string &Value )
{
  setVariableValueString( VarName, Value );
}

// -----------------------------------------------------

void configParserStruct::parser::setVariableValue( const std::string &VarName, int Value )
{
  setVariableValue( VarName, convertToString(Value) ); 
}

// -----------------------------------------------------

void configParserStruct::parser::setVariableValue( const std::string &VarName, double Value )
{
  setVariableValue( VarName, convertToString(Value) ); 
}

// -----------------------------------------------------

double configParserStruct::parser::doubleVariable( const std::string &VarName, double DefaultValue ) const  
{ 
  const std::string &Value = stringVariable( VarName, convertToString(DefaultValue) );
  return convertFromString<double>( Value ); 
}

// -----------------------------------------------------

int configParserStruct::parser::integerVariable( const std::string &VarName, int DefaultValue  ) const  
{ 
  const std::string &Value = stringVariable( VarName, convertToString(DefaultValue) );
  return convertFromString<int>( Value ); 
}

// -----------------------------------------------------

configParserStruct::parser::containerForVariables configParserStruct::parser::listOfVariablesStruct() const
{
  const containerForVariables Variables = listOfVariables();
  containerForVariables Result;

  for ( containerForVariables::const_iterator i = Variables.begin(); i != Variables.end(); ++i )
  {
    size_t LastPointPos = std::string::npos;
    do
    {
      LastPointPos = i->rfind( structSeparator(), LastPointPos );
      if ( LastPointPos == std::string::npos || LastPointPos == 0 )
        break;
      std::string StructName = i->substr( 0, LastPointPos );
      Result.insert( StructName );
      LastPointPos -= 1;
    } while ( true );
  }

  return Result;
}

// =====================================================



