
// =====================================================

#include "configParserStruct/structparservars.h"

#include <typeinfo>

// =====================================================

bool configParserStruct::structParserUtil::variable::isDefined() const 
{ 
  return typeid(*Value) != typeid(undefVariableValue); 
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


