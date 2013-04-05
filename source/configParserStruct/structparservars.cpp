
// =====================================================

#include "configParserStruct/structparservars.h"

// =====================================================

void configParserStruct::structParser::variablesList::set( const std::string &Name, const variable &Var )
{
  Variables[ Name ] = Var;
}

// -----------------------------------------------------

const configParserStruct::structParser::variable configParserStruct::structParser::variablesList::get( const std::string &Name ) const
{
  std::map< std::string, variable >::const_iterator Iterator = Variables.find( Name );
  if ( Iterator == Variables.end() )
    return variable();
  return Iterator->second;
}

// =====================================================

void configParserStruct::structParser::variablesStack::push( const variable &Var )
{
  Stack.push_back( Var );
}

// -----------------------------------------------------

const configParserStruct::structParser::variable configParserStruct::structParser::variablesStack::pop()
{
  if ( Stack.empty() )
    return variable();

  variable Top = Stack.back();
  Stack.pop_back();
  return Top;
}

// -----------------------------------------------------

const configParserStruct::structParser::variable configParserStruct::structParser::variablesStack::top() const
{
  if ( Stack.empty() )
    return variable();
  return Stack.back();
}

// =====================================================


