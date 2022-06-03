
// =====================================================

#include "configParserStruct/structparser.h"
#include "configParserStruct/structparserprogram.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cmath>

// =====================================================
        
configParserStruct::structParser::exception::exception( const std::string &What ) : 
  ::configParserStruct::exception(What) 
{
}

// =====================================================

configParserStruct::structParser::parsingException::parsingException( int L ) :
  exception( "Error while parsing in line " + convertToString( L ) ),
  Line( L )
{
}

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
  build( Text );
  run();
}

// -----------------------------------------------------

void configParserStruct::structParser::build( const std::string &Text )
{
  assert( Program != NULL );
  Program->initBuiltInVariables();
  bool OK = Program->build( Text );
  if ( ! OK )
    throw parsingException( Program->errorLine() );
  ListOfVariables.clear();
}

// -----------------------------------------------------

void configParserStruct::structParser::run()
{
  Program->execute();
  ListOfVariables.clear();
}

// -----------------------------------------------------

void configParserStruct::structParser::reset()
{
  assert( Program != NULL );
  Program->clear();
  Program->initBuiltInVariables();
  ListOfVariables.clear();
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
  if ( ! ListOfVariables.empty() )
    return ListOfVariables;


  const std::set<std::string> &BuildVariables = Program->onBuildVariableNames();
  ListOfVariables.insert( BuildVariables.begin(), BuildVariables.end() );
  
  const std::set<std::string> &OnExecute = Program->onExecuteVariableNames();
  ListOfVariables.insert( OnExecute.begin(), OnExecute.end() );

  for ( containerForVariables::iterator s = ListOfVariables.begin(); s != ListOfVariables.end(); )
  {
    if ( s->find( structParserUtil::program::lastResultVariableName() ) == 0 )
    {
      containerForVariables::iterator es = s++;
      ListOfVariables.erase( es );
    } else {
      ++s;
    }
  }

  ListOfVariables.erase( structParserUtil::program::lastResultVariableName() );
  return ListOfVariables;
}
// -----------------------------------------------------
      
void configParserStruct::structParser::setVariableValue( const std::string &VarName, const std::string &Value )
{
  Program->setNamedVariable( VarName, structParserUtil::createVariable(Value) );
  ListOfVariables.clear();
}

// -----------------------------------------------------

void configParserStruct::structParser::setVariableValue( const std::string &VarName, int Value )
{
  Program->setNamedVariable( VarName, structParserUtil::createVariable(Value) );
  ListOfVariables.clear();
}

// -----------------------------------------------------

void configParserStruct::structParser::setVariableValue( const std::string &VarName, double Value )
{
  Program->setNamedVariable( VarName, structParserUtil::createVariable(Value) );
  ListOfVariables.clear();
}

// =====================================================

