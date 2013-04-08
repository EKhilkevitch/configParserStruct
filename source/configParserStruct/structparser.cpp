
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
  Program->initBuiltInVariables();
  Program->build( Text );
  Program->execute();
}

// -----------------------------------------------------

void configParserStruct::structParser::reset()
{
  assert( Program != NULL );
  Program->clear();
  Program->initBuiltInVariables();
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
  const structParserUtil::variable& Variable = Program->getNamedVariable( VarName );
  return Variable.isDefined() ? Variable.integer() : DefaultValue;
}

// -----------------------------------------------------

double configParserStruct::structParser::lastDoubleVariable() const
{
  const structParserUtil::variable& Variable = Program->getLastExpressionReuslt();
  return Variable.number();
}

// -----------------------------------------------------
      
configParserStruct::structParser::containerForVariables configParserStruct::structParser::listOfVariables() const
{
  containerForVariables Result;
  const std::list<std::string> &List = Program->variableNames();
  for ( std::list<std::string>::const_iterator i = List.begin(); i != List.end(); ++i )
  {
    if ( i->find( structParserUtil::program::lastResultVariableName() ) == 0 )
      continue;
    Result.insert( *i );
  }
  return Result;
}
// -----------------------------------------------------
      
void configParserStruct::structParser::setVariableValue( const std::string &VarName, const std::string &Value )
{
  Program->setNamedVariable( VarName, structParserUtil::createVariable(Value) );
}

// -----------------------------------------------------

void configParserStruct::structParser::setVariableValue( const std::string &VarName, int Value )
{
  Program->setNamedVariable( VarName, structParserUtil::createVariable(Value) );
}

// -----------------------------------------------------

void configParserStruct::structParser::setVariableValue( const std::string &VarName, double Value )
{
  Program->setNamedVariable( VarName, structParserUtil::createVariable(Value) );
}

// =====================================================

