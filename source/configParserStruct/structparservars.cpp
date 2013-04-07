
// =====================================================

#include "configParserStruct/structparservars.h"

#include <typeinfo>
#include <set>

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

// -----------------------------------------------------
        
configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::variablesStack::topPointer()
{
  if ( Stack.empty() )
    return NULL;
  return &Stack.back();
}

// =====================================================

void configParserStruct::structParserUtil::variablesListStack::set( const std::string &Name, const variable &Var )
{
  assert( ! Stack.empty() );

  const std::string &GlobalName = globalName(Name);

  if ( GlobalName.empty() )
    Stack.back().set( Name, Var );
  else
    Stack.front().set( GlobalName, Var );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesListStack::get( const std::string &Name ) const
{
  assert( ! Stack.empty() );

  const std::string &GlobalName = globalName(Name);

  if ( GlobalName.empty() )
  {
    for ( varListStack::const_reverse_iterator s = Stack.rbegin(); s != Stack.rend(); ++s )
    {
      variable Variable = s->get( Name );
      if ( Variable.isDefined() )
        return Variable;
    }
    return variable();
  } else {
    return Stack.front().get( GlobalName );
  }
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::variablesListStack::popList()
{
  if ( Stack.size() > 1 )
    Stack.pop_back();
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variablesListStack::clear()
{
  Stack.clear();
  Stack.push_back( variablesList() );
}

// -----------------------------------------------------

std::list<std::string> configParserStruct::structParserUtil::variablesListStack::listOfNames() const
{
  std::set<std::string> Result;
  for ( varListStack::const_iterator s = Stack.begin(); s != Stack.end(); ++s )
  {
    std::list<std::string> Names = s->listOfNames();
    Result.insert( Names.begin(), Names.end() );
  }
  return std::list<std::string>( Result.begin(), Result.end() );
}

// -----------------------------------------------------

std::list<std::string> configParserStruct::structParserUtil::variablesListStack::listOfNamesInAllStack() const
{
  std::set<std::string> Result;
  for ( varListStack::const_iterator s = Stack.begin(); s != Stack.end(); ++s )
  {
    std::list<std::string> Names = s->listOfNamesIncludeSubdict();
    Result.insert( Names.begin(), Names.end() );
  }
  return std::list<std::string>( Result.begin(), Result.end() );
}

// -----------------------------------------------------

std::string configParserStruct::structParserUtil::variablesListStack::globalName( const std::string &Name )
{
  size_t Pos = Name.find( globalPrefix() );
  if ( Pos == 0 )
    return Name.substr( globalPrefix().size(), std::string::npos );
  return std::string();
}

// =====================================================


