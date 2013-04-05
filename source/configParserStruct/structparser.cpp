
// =====================================================

#include "configParserStruct/structparser.h"
#include "configParserStruct/structparserprogram.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cmath>

// =====================================================

configParserStruct::structParser::structParser( const std::string &ConfigText ) :
  Program(NULL)
{
  Program = new structParserUtil::program();
  if ( ! ConfigText.empty() )
    exec( ConfigText );
}

// -----------------------------------------------------

configParserStruct::structParser::~structParser()
{
  delete Program;
}

// -----------------------------------------------------

void configParserStruct::structParser::exec( const std::string &Text )
{
  assert( Program != NULL );
  Program->build( Text );
  Program->execute();
}

// -----------------------------------------------------
      
bool configParserStruct::structParser::isVariableExist( const std::string &VarName ) const
{
  return Program->getNamedVariable( VarName ).isDefined();
}

// -----------------------------------------------------

std::string configParserStruct::structParser::stringVariable( const std::string &VarName, const std::string &DefaultValue ) const
{
  const structParserUtil::variable& Variable = Program->getNamedVariable( VarName );
  return Variable.isDefined() ? Variable.string() : DefaultValue;
}

// -----------------------------------------------------
      
double configParserStruct::structParser::doubleVariable( const std::string &VarName, double DefaultValue ) const
{
  const structParserUtil::variable& Variable = Program->getNamedVariable( VarName );
  return Variable.isDefined() ? Variable.number() : DefaultValue;
}

// -----------------------------------------------------

int configParserStruct::structParser::integerVariable( const std::string &VarName, int DefaultValue ) const
{
  return round( doubleVariable( VarName, DefaultValue ) );
}

// -----------------------------------------------------
      
configParserStruct::structParser::containerForVariables configParserStruct::structParser::listOfVariables() const
{
  containerForVariables Result;
  const std::list<std::string> &List = Program->variableNames();
  for ( std::list<std::string>::const_iterator i = List.begin(); i != List.end(); ++i )
    Result.insert( *i );
  return Result;
}

// =====================================================

