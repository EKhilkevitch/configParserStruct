
// =====================================================

#include "configParserStruct/parser.h"
#include "configParserStruct/program.h"
#include "configParserStruct/variable.h"
#include "configParserStruct/variablevalue.h"
#include "configParserStruct/exception.h"

#include <cassert>
#include <stdexcept>

// =====================================================
      
const char *const configParserStruct::parser::LastExpressionValueName = "..LEXP..";
const int configParserStruct::parser::SuccesssErrorLine = -1;

// -----------------------------------------------------

configParserStruct::parser::parser() :
  Program( new program() )
{
}

// -----------------------------------------------------
      
configParserStruct::parser::parser( const parser &Other ) :
  Program( new program(*Other.Program) )
{
}

// -----------------------------------------------------

configParserStruct::parser& configParserStruct::parser::operator=( const parser &Other )
{
  if ( &Other == this )
    return *this;

  parser Copy(Other);
  swap( Copy );

  return *this;
}

// -----------------------------------------------------

configParserStruct::parser::~parser()
{
  delete Program;
}

// -----------------------------------------------------
      
void configParserStruct::parser::swap( parser &Other )
{
  program *Tmp = Other.Program;
  Other.Program = Program;
  Program = Tmp;
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
      
enum configParserStruct::parser::variableType configParserStruct::parser::variableType( const std::string &Name ) const
{
  const variable *Variable = ( Name == LastExpressionValueName ) ? 
    &Program->programMemory().lastResult() :
    Program->programMemory().findValueByName(Name,named::GlobalScope);

  if ( Variable == NULL )
    return VarNone;

  const char *Type = Variable->type();
  if ( Type == undefVariableValue::TypeName )
    return VarUndef;
  if ( Type == integerVariableValue::TypeName )
    return VarInteger;
  if ( Type == realVariableValue::TypeName )
    return VarReal;
  if ( Type == stringVariableValue::TypeName )
    return VarString;
  if ( Type == arrayVariableValue::TypeName )
    return VarArray;
  if ( Type == dictVariableValue::TypeName )
    return VarDict;
  return VarOther;
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

// -----------------------------------------------------
      
std::set<std::string> configParserStruct::parser::variables() const
{
  return Program->programText().variables();
}

// -----------------------------------------------------

int configParserStruct::parser::errorLine() const
{
  const int Line = Program->programText().errorLine();
  if ( Line == text::SuccesssErrorLine )
    return SuccesssErrorLine;
  return Line;
}

// =====================================================

