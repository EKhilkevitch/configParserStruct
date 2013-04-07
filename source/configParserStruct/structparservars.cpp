
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
  size_t DelimiterPos = Key.find_first_of(".");
  if ( DelimiterPos == std::string::npos )
  {
    Dict[ Key ] = Value;
  } else {
    std::string ItemKey = Key.substr( 0, DelimiterPos );
    std::string ItemName = Key.substr( DelimiterPos+1, std::string::npos );
    if ( Dict[ItemKey].valueType() != typeid(dictVariableValue) )
      Dict[ItemKey] = dictVariableValue();
    Dict[ItemKey].value<dictVariableValue>().addItem( ItemName, Value );
  }
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::dictVariableValue::getItem( const std::string &Key ) const
{
  std::map< std::string, variable >::const_iterator Iterator = Dict.find( Key );
  if ( Iterator == Dict.end() )
    return variable();
  return Iterator->second;
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::dictVariableValue::getItemPointer( const std::string &Key ) 
{
  std::map< std::string, variable >::iterator Iterator = Dict.find( Key );
  if ( Iterator == Dict.end() )
    return NULL;
  return &Iterator->second;
}

// -----------------------------------------------------
        
const std::list<std::string> configParserStruct::structParserUtil::dictVariableValue::listOfKeys() const
{
  std::list<std::string> List;
  for ( std::map< std::string, variable >::const_iterator i = Dict.begin(); i != Dict.end(); ++i )
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


