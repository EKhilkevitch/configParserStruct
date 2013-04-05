
// =====================================================

#include "configParserStruct/structparservars.h"

#include <typeinfo>

// =====================================================

bool configParserStruct::structParserUtil::variable::isDefined() const 
{ 
  return typeid(*Value) != typeid(undefVariableValue); 
}

// =====================================================

const std::string configParserStruct::structParserUtil::dictVariableValue::string() const
{
  std::string Result;
  Result += "{ ";
  for ( std::map< std::string, variable >::const_iterator i = Dict.begin(); i != Dict.end(); ++i )
  {
    if ( i != Dict.begin() )
      Result += ", ";
    Result += "." + i->first + " = " + i->second.string();
  }
  Result += " }";
  return Result;
};
        
// -----------------------------------------------------

void configParserStruct::structParserUtil::dictVariableValue::addItem( const std::string &Key, const variable &Value )
{
  Dict[ Key ] = Value;
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::dictVariableValue::getItem( const std::string &Key ) const
{
  std::map< std::string, variable >::const_iterator Iterator = Dict.find( Key );
  if ( Iterator == Dict.end() )
    return variable();
  return Iterator->second;
}

// =====================================================

void configParserStruct::structParserUtil::variablesList::set( const std::string &Name, const variable &Var )
{
  Variables[ Name ] = Var;
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesList::get( const std::string &Name ) const
{
  std::map< std::string, variable >::const_iterator Iterator = Variables.find( Name );
  if ( Iterator == Variables.end() )
    return variable();
  return Iterator->second;
}

// -----------------------------------------------------
        
std::list<std::string> configParserStruct::structParserUtil::variablesList::listOfNames() const
{
  std::list<std::string> List;
  for ( std::map< std::string, variable >::const_iterator i = Variables.begin(); i != Variables.end(); ++i )
    List.push_back( i->first );
  return List;
}

// =====================================================

void configParserStruct::structParserUtil::variablesStack::push( const variable &Var )
{
  Stack.push_back( Var );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesStack::pop()
{
  if ( Stack.empty() )
    return variable();

  variable Top = Stack.back();
  Stack.pop_back();
  return Top;
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesStack::top() const
{
  if ( Stack.empty() )
    return variable();
  return Stack.back();
}

// =====================================================


