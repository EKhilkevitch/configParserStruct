
// =====================================================

#include "configParserStruct2/parser.h"
#include "configParserStruct2/program.h"
#include "configParserStruct2/variable.h"
#include "configParserStruct2/exception.h"

#include <cassert>
#include <stdexcept>

// =====================================================
      
const char *const configParserStruct::parser::LastExpressionValueName = "..LEXP..";

// -----------------------------------------------------

configParserStruct::parser::parser() :
  Program( new program() )
{
}

// -----------------------------------------------------

configParserStruct::parser::~parser()
{
  delete Program;
}

// -----------------------------------------------------

void configParserStruct::parser::exec( const std::string &Text )
{
  build( Text );
  run();
}

// -----------------------------------------------------

void configParserStruct::parser::build( const std::string &Text )
{
  Program->build(Text);
}

// -----------------------------------------------------

void configParserStruct::parser::run()
{
  Program->run();
}

// -----------------------------------------------------

void configParserStruct::parser::reset()
{
  Program->clear();
}

// -----------------------------------------------------
      
bool configParserStruct::parser::isVariableExist( const std::string &Name ) const
{
  if ( Name == LastExpressionValueName )
    return true;

  const variable *Variable = Program->programMemory().findValueByName(Name,named::GlobalScope);

  if ( Variable == NULL )
    return false;
  else
    return true;
}

// -----------------------------------------------------

std::string configParserStruct::parser::stringVariable( const std::string &Name, const std::string &DefaultValue ) const
{
  if ( Name == LastExpressionValueName )
    return Program->programMemory().lastResult().string();

  const variable *Variable = Program->programMemory().findValueByName(Name,named::GlobalScope);
  if ( Variable == NULL )
    return DefaultValue;
  return Variable->string();
}

// -----------------------------------------------------
      
double configParserStruct::parser::doubleVariable( const std::string &Name, double DefaultValue ) const
{
  if ( Name == LastExpressionValueName )
    return Program->programMemory().lastResult().real();

  const variable *Variable = Program->programMemory().findValueByName(Name,named::GlobalScope);
  if ( Variable == NULL )
    return DefaultValue;
  return Variable->real();
}

// -----------------------------------------------------

int configParserStruct::parser::integerVariable( const std::string &Name, int DefaultValue ) const
{
  if ( Name == LastExpressionValueName )
    return Program->programMemory().lastResult().integer();

  const variable *Variable = Program->programMemory().findValueByName(Name,named::GlobalScope);
  if ( Variable == NULL )
    return DefaultValue;
  return Variable->integer();
}

// -----------------------------------------------------
      
void configParserStruct::parser::setVariable( const std::string &Name, const std::string &Value )
{
  if ( Name == LastExpressionValueName )
    throw exception( "Can not explicit set last expression value" );

  Program->programMemory().setValueByName( Name, variable(Value.c_str()), named::GlobalScope );
}

// -----------------------------------------------------

void configParserStruct::parser::setVariable( const std::string &Name, int Value )
{
  if ( Name == LastExpressionValueName )
    throw exception( "Can not explicit set last expression value" );

  Program->programMemory().setValueByName( Name, variable(Value), named::GlobalScope );
}

// -----------------------------------------------------

void configParserStruct::parser::setVariable( const std::string &Name, double Value )
{
  if ( Name == LastExpressionValueName )
    throw exception( "Can not explicit set last expression value" );

  Program->programMemory().setValueByName( Name, variable(Value), named::GlobalScope );
}

// =====================================================

