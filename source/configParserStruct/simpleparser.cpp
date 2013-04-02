
// =====================================================

#include "configParserStruct/simpleparser.h"

#include <cassert>
#include <stdexcept>
#include <iostream>

// =====================================================

configParserStruct::simpleParser::simpleParser( const std::string &ConfigText )
{
  if ( ! ConfigText.empty() )
    exec( ConfigText );
}

// -----------------------------------------------------

bool configParserStruct::simpleParser::isVariableExist( const std::string &VarName ) const
{
  return Variables.find( VarName ) != Variables.end(); 
}

// -----------------------------------------------------
      
configParserStruct::simpleParser::containerForVariables configParserStruct::simpleParser::listOfVariables() const
{
  containerForVariables Result;
  for ( std::map<std::string,std::string>::const_iterator v = Variables.begin(); v != Variables.end(); ++v )
    Result.insert( v->first );
  return Result;
}

// -----------------------------------------------------
      
void configParserStruct::simpleParser::exec( const std::string &ConfigText )
{
  const std::list<std::string> &Lines = splitString( ConfigText, "\n;" );
  for ( std::list<std::string>::const_iterator s = Lines.begin(); s != Lines.end(); ++s )
  {
    std::pair<std::string,std::string> NameAndValue = splitLineOnNameandValue( *s );
    if ( ! NameAndValue.first.empty() )
      Variables.insert( NameAndValue );
  }
}

// -----------------------------------------------------
      
void configParserStruct::simpleParser::setVariableValueString( const std::string &VarName, const std::string &Value ) 
{
  Variables[ VarName ] = Value;
}

// -----------------------------------------------------

std::string configParserStruct::simpleParser::stringVariable( const std::string &VarName, const std::string &DefaultValue ) const 
{
  std::map<std::string,std::string>::const_iterator Iterator = Variables.find( VarName );
  if ( Iterator == Variables.end() )
    return DefaultValue;
  return Iterator->second;
}

// -----------------------------------------------------

std::pair<std::string,std::string> configParserStruct::simpleParser::splitLineOnNameandValue( const std::string &OneLine )
{
  std::string String = OneLine;

  size_t CommentPos = String.find( "#" );
  String = String.substr( 0, CommentPos );
    
  size_t EqPos = String.find( "=" );
  if ( EqPos == std::string::npos )
    return std::make_pair( std::string(), std::string() );

  std::string VarName  = stripSpaces( String.substr( 0, EqPos ) );
  std::string VarValue = stripSpaces( String.substr( EqPos + 1, std::string::npos ) );

  return std::make_pair( VarName, VarValue );
}

// -----------------------------------------------------
      
std::string configParserStruct::simpleParser::stripSpaces( const std::string &String )
{
  const char *Spaces = "\n\r\t ";
  size_t Begin = String.find_first_not_of( Spaces );
  size_t End   = String.find_last_not_of( Spaces );
  return String.substr( Begin, End + 1 - Begin );
}

// =====================================================
      

