
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
    Result += dictSeparator() + i->first + " = " + i->second.string();
  }
  Result += " }";
  return Result;
};
        
// -----------------------------------------------------

std::pair<std::string,std::string> configParserStruct::structParserUtil::dictVariableValue::splitKey( const std::string &Key )
{
  assert( dictSeparator().length() == 1 );
  size_t DelimiterPos = Key.find_first_of( dictSeparator() );
  if ( DelimiterPos == std::string::npos )
    return std::make_pair( Key, std::string() );
  return std::make_pair( Key.substr( 0, DelimiterPos ), Key.substr( DelimiterPos+1, std::string::npos ) );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::dictVariableValue::addItem( const std::string &Key, const variable &Value )
{
  std::pair<std::string,std::string> KeyParts = splitKey(Key);
  if ( KeyParts.second.empty() )
  {
    Dict[ Key ] = Value;
  } else {
    if ( Dict[ KeyParts.first ].valueType() != typeid(dictVariableValue) )
      Dict[ KeyParts.first ] = dictVariableValue();
    Dict[ KeyParts.first ].value<dictVariableValue>().addItem( KeyParts.second, Value );
  }
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::dictVariableValue::getItem( const std::string &Key ) const
{
  const variable *Pointer = const_cast< dictVariableValue* >(this)->getItemPointer(Key);
  if ( Pointer == NULL )
    return variable();
  return *Pointer;
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::dictVariableValue::getItemPointer( const std::string &Key ) 
{
  std::pair<std::string,std::string> KeyParts = splitKey(Key);
  std::map< std::string, variable >::iterator Iterator = Dict.find( KeyParts.first );
  if ( Iterator == Dict.end() )
    return NULL;
  if ( KeyParts.second.empty() )
    return &Iterator->second;
  if ( Dict[ KeyParts.first ].valueType() != typeid(dictVariableValue) )
    return NULL;
  return Dict[ KeyParts.first ].value<dictVariableValue>().getItemPointer( KeyParts.second );
}

// -----------------------------------------------------
        
const std::list<std::string> configParserStruct::structParserUtil::dictVariableValue::listOfKeys() const
{
  std::list<std::string> List;
  for ( std::map< std::string, variable >::const_iterator i = Dict.begin(); i != Dict.end(); ++i )
    List.push_back( i->first );
  return List;
}

// -----------------------------------------------------
        
const std::list<std::string> configParserStruct::structParserUtil::dictVariableValue::listOfKeysIncludeSubdict() const
{
  std::list<std::string> List;
  for ( std::map< std::string, variable >::const_iterator i = Dict.begin(); i != Dict.end(); ++i )
  {
    if ( i->second.valueType() == typeid(dictVariableValue) )
    {
      std::list<std::string> DictKeys = i->second.value<dictVariableValue>().listOfKeysIncludeSubdict();
      for ( std::list<std::string>::const_iterator s = DictKeys.begin(); s != DictKeys.end(); ++s )
        List.push_back( i->first + dictSeparator() + *s );
    } else {
      List.push_back( i->first );
    }
  }
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


