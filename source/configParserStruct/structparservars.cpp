
// =====================================================

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparserprogram.h"

#include <typeinfo>
#include <set>
#include <iostream>
#include <cstdlib>

// =====================================================
        
configParserStruct::structParserUtil::variableValue::~variableValue() 
{
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variableValue::valueByReference( const variable &Reference, unsigned AttrLevel ) const
{
  if ( AttrLevel == Reference.value<referenceVariableValue>().numberOfAttributes() )
    return *this;
  return variable();
}

// =====================================================
        
configParserStruct::structParserUtil::variable::variable() 
{
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable::variable( const variableValue &V ) : 
  Value(V) 
{
}

// -----------------------------------------------------

const std::string configParserStruct::structParserUtil::variable::string() const 
{ 
  return Value->string(); 
}

// -----------------------------------------------------

double configParserStruct::structParserUtil::variable::number() const 
{ 
  return Value->number(); 
}

// -----------------------------------------------------

int configParserStruct::structParserUtil::variable::integer() const 
{ 
  return Value->integer(); 
}

// -----------------------------------------------------

bool configParserStruct::structParserUtil::variable::boolean() const 
{ 
  return Value->boolean(); 
}

// -----------------------------------------------------

bool configParserStruct::structParserUtil::variable::isDefined() const 
{ 
  return typeid(*Value) != typeid(undefVariableValue); 
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variable::valueByReference( const variable &Reference, unsigned AttrLevel ) const
{
  return Value->valueByReference(Reference,AttrLevel); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variable::setValueByReference( const variable &Reference, const variable &Value, unsigned AttrLevel )
{
  const referenceVariableValue &RefValue = Reference.value<referenceVariableValue>();

  if ( AttrLevel == RefValue.numberOfAttributes() )
  {
    *this = Value;
    return;
  }

  if ( AttrLevel < RefValue.numberOfAttributes() )
  {
    if ( ! isValueDerivedFrom<composerVariableValue>() )
    {
      if ( RefValue.attribute(AttrLevel).valueType() == typeid(stringVariableValue) )
        *this = dictVariableValue();
      if ( RefValue.attribute(AttrLevel).valueType() == typeid(integerVariableValue) )
        *this = arrayVariableValue();
      if ( RefValue.attribute(AttrLevel).valueType() == typeid(realVariableValue) )
        *this = arrayVariableValue();
    }

    if ( isValueDerivedFrom<composerVariableValue>() )
      value<composerVariableValue>().setValueByReference( Reference, Value, AttrLevel );
    else
      *this = undefVariableValue();
    return;
  }

  *this = undefVariableValue();
}

// =====================================================

size_t configParserStruct::structParserUtil::builtinFunctionValue::getNumberOfArguments( const program &Program ) 
{
  return Program.getNamedVariableFromTopOfStack( "$N" ).integer();
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::builtinFunctionValue::getArgument( size_t Index, const program &Program ) 
{
  std::string Name = "$" + convertToString(Index);
  return Program.getNamedVariableFromTopOfStack( Name );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::builtinFunctionValue::getVariable( const std::string &Name, const program &Program ) 
{
  return Program.getNamedVariable( Name );
}

// =====================================================

const std::string configParserStruct::structParserUtil::referenceVariableValue::string() const
{
  std::string Result;
  Result += "Ref: '" + Name + "'";
  Result += " << ";
  for ( std::vector< variable >::const_iterator i = Attributes.begin(); i != Attributes.end(); ++i )
  {
    if ( i != Attributes.begin() )
      Result += ", ";
    Result += i->string();
  }
  Result += " >>";
  return Result;
}

// -----------------------------------------------------
        
const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::referenceVariableValue::getValue( const program &Program ) const
{
  return Program.getNamedVariable( Name ).valueByReference( *this );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::referenceVariableValue::setValue( program *Program, const variable &Value ) const
{
  assert( Program != NULL );

  variable *Variable = Program->getNamedVariablePointer( Name );
  if ( Variable == NULL )
  {
    Program->setNamedVariable( Name, variable() );
    Variable = Program->getNamedVariablePointer( Name );
    assert( Variable != NULL );
  }
    
  Variable->setValueByReference( *this, Value );
}

// =====================================================
        
const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::composerVariableValue::valueByReference( const variable &Reference, unsigned AttrLevel ) const
{
  const referenceVariableValue &RefValue = Reference.value<referenceVariableValue>();

  if ( AttrLevel > RefValue.numberOfAttributes() )
    return variable();

  if ( AttrLevel == RefValue.numberOfAttributes() )
    return *this;

  const variable &Key = RefValue.attribute( AttrLevel );
  return getItemByVariableKey( Key ).valueByReference( Reference, AttrLevel + 1 );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::composerVariableValue::setValueByReference( const variable &Reference, const variable &Value, unsigned AttrLevel )
{
  const referenceVariableValue &RefValue = Reference.value<referenceVariableValue>();

  assert( AttrLevel < RefValue.numberOfAttributes() );

  const variable &Key = RefValue.attribute( AttrLevel );
  variable *Item = getItemPointerByVariableKey( Key );
//  std::cerr << "setValueByReference: " << Value.string() << " " << Reference.string() << " " << AttrLevel << " " << Key.string() << " " << Item << std::endl;
  if ( Item == NULL )
  {
    addItemByVariableKey( Key, variable() );
    Item = getItemPointerByVariableKey( Key );
    assert( Item != NULL );
  }
  Item->setValueByReference( Reference, Value, AttrLevel + 1 );
}

// -----------------------------------------------------

std::pair<std::string,std::string> configParserStruct::structParserUtil::composerVariableValue::splitKey( const std::string &Key )
{
  const size_t DelimiterDictPos = Key.find( dictVariableValue::dictSeparator() );
  const size_t DelimiterArrayBeginPos = Key.find( arrayVariableValue::arraySeparatorBegin() );
  const size_t DelimiterArrayEndPos = ( DelimiterArrayBeginPos == std::string::npos ) ? std::string::npos : Key.find( arrayVariableValue::arraySeparatorEnd() );

  if ( DelimiterDictPos != std::string::npos && DelimiterDictPos < DelimiterArrayBeginPos )
    return std::make_pair( Key.substr( 0, DelimiterDictPos ), Key.substr( DelimiterDictPos+1, std::string::npos ) );

  if ( DelimiterArrayBeginPos != std::string::npos && DelimiterArrayEndPos != std::string::npos )
    return std::make_pair( Key.substr( 0, DelimiterArrayBeginPos ), Key.substr( DelimiterArrayBeginPos+1, std::string::npos ) );
  
  return std::make_pair( Key, std::string() );
}

// -----------------------------------------------------
        
configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::composerVariableValue::getItemPointer( variable *Variable, const std::string &KeySuffix )
{
  if ( KeySuffix.empty() )
    return Variable;

  if ( Variable == NULL )
    return Variable;

  std::pair<std::string,std::string> KeyParts = splitKey(KeySuffix);

  if ( Variable->valueType() == typeid(dictVariableValue) )
  {
    Variable = Variable->value<dictVariableValue>().getItemPointerNotFollow( KeyParts.first );
    return composerVariableValue::getItemPointer( Variable, KeyParts.second );
  }

  if ( Variable->valueType() == typeid(arrayVariableValue) )
  {
    int Index = std::atoi( KeyParts.first.c_str() );
    Variable = Variable->value<arrayVariableValue>().getItemPointer( Index );
    return composerVariableValue::getItemPointer( Variable, KeyParts.second );
  }

  return NULL;
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
}
        
// -----------------------------------------------------

char configParserStruct::structParserUtil::dictVariableValue::dictSeparator() 
{ 
  return '.'; 
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
        
void configParserStruct::structParserUtil::dictVariableValue::removeItem( const std::string &Key )
{
  std::pair<std::string,std::string> KeyParts = splitKey(Key);

  if ( KeyParts.second.empty() )
  {
    Dict.erase( Key );
  } else {
    if ( Dict[ KeyParts.first ].valueType() != typeid(dictVariableValue) )
      return;
    Dict[ KeyParts.first ].value<dictVariableValue>().removeItem( KeyParts.second );
  }

}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::dictVariableValue::getItemPointerNotFollow( const std::string &Key )
{
  std::map< std::string, variable >::iterator Iterator = Dict.find( Key );
  if ( Iterator == Dict.end() )
    return NULL;
  return &( Iterator->second );
}

// -----------------------------------------------------

size_t configParserStruct::structParserUtil::dictVariableValue::numberOfItems() const 
{ 
  return Dict.size(); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::dictVariableValue::clear() 
{ 
  Dict.clear(); 
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::dictVariableValue::getItemPointerByVariableKey( const variable &Key ) 
{ 
  return getItemPointer( Key.string() ); 
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::dictVariableValue::getItemByVariableKey( const variable &Key ) const 
{ 
  return getItem( Key.string() ); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::dictVariableValue::addItemByVariableKey( const variable &Key, const variable &Value )
{ 
  addItem( Key.string(), Value ); 
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

  variable *Item = &( Iterator->second );
  
  return composerVariableValue::getItemPointer( Item, KeyParts.second );
}

// -----------------------------------------------------
        
const std::set<std::string> configParserStruct::structParserUtil::dictVariableValue::listOfKeys() const
{
  std::set<std::string> Set;
  for ( std::map< std::string, variable >::const_iterator i = Dict.begin(); i != Dict.end(); ++i )
    Set.insert( i->first );
  return Set;
}

// -----------------------------------------------------
        
const std::set<std::string> configParserStruct::structParserUtil::dictVariableValue::listOfKeysIncludeSubdict() const
{
  std::set<std::string> Set;
  for ( std::map< std::string, variable >::const_iterator i = Dict.begin(); i != Dict.end(); ++i )
  {
    if ( i->second.valueType() == typeid(dictVariableValue) )
    {
      const std::set<std::string> &DictKeys = i->second.value<dictVariableValue>().listOfKeysIncludeSubdict();
      for ( std::set<std::string>::const_iterator s = DictKeys.begin(); s != DictKeys.end(); ++s )
        Set.insert( i->first + dictSeparator() + *s );
    } else {
      Set.insert( i->first );
    }
  }
  return Set;
}

// =====================================================
        
const std::string configParserStruct::structParserUtil::arrayVariableValue::string() const
{
  std::string Result;
  Result += "[ ";
  for ( std::deque< variable >::const_iterator i = Array.begin(); i != Array.end(); ++i )
  {
    if ( i != Array.begin() )
      Result += ", ";
    Result += i->string();
  }
  Result += " ]";
  return Result;
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::arrayVariableValue::addItem( size_t Index, const variable &Value )
{
  if ( Array.size() <= Index )
    Array.resize( Index+1, variable() );
  Array.at( Index ) = Value;
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::arrayVariableValue::pushItem( const variable &Value ) 
{ 
  addItem( numberOfItems(), Value ); 
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::arrayVariableValue::getItem( int Index ) const
{
  const variable *Pointer = const_cast< arrayVariableValue* >(this)->getItemPointer(Index);
  if ( Pointer == NULL )
    return variable();
  return *Pointer;
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::arrayVariableValue::getItemPointer( int Index )
{
  if ( Index >= static_cast<int>(Array.size()) || Index < 0 )
    return NULL;
  return &Array[Index];
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::arrayVariableValue::getItemPointerByVariableKey( const variable &Key ) 
{ 
  return getItemPointer( Key.integer() ); 
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::arrayVariableValue::getItemByVariableKey( const variable &Key ) const 
{ 
  return getItem( Key.integer() ); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::arrayVariableValue::addItemByVariableKey( const variable &Key, const variable &Value ) 
{ 
  addItem( Key.integer(), Value ); 
}

// -----------------------------------------------------
        
char configParserStruct::structParserUtil::arrayVariableValue::arraySeparatorBegin()
{
  return '[';
}

// -----------------------------------------------------

char configParserStruct::structParserUtil::arrayVariableValue::arraySeparatorEnd()
{
  return ']';
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

configParserStruct::structParserUtil::variablesList::variablesList()
{
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variablesList::~variablesList()
{
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variablesList::set( const std::string &Name, const variable &Var ) 
{ 
  Dict.addItem( Name, Var ); 
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesList::get( const std::string &Name ) const 
{ 
  return Dict.getItem( Name ); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variablesList::remove( const std::string &Name ) 
{
  Dict.removeItem( Name );
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::variablesList::getPointer( const std::string &Name ) 
{ 
  return Dict.getItemPointer( Name ); 
}

// -----------------------------------------------------

std::set<std::string> configParserStruct::structParserUtil::variablesList::listOfNames() const 
{ 
  return Dict.listOfKeys(); 
}

// -----------------------------------------------------

std::set<std::string> configParserStruct::structParserUtil::variablesList::listOfNamesIncludeSubdict() const 
{ 
  return Dict.listOfKeysIncludeSubdict(); 
}

// -----------------------------------------------------

size_t configParserStruct::structParserUtil::variablesList::size() const 
{ 
  return Dict.numberOfItems(); 
}

// -----------------------------------------------------

bool configParserStruct::structParserUtil::variablesList::empty() const 
{ 
  return Dict.numberOfItems() <= 0; 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variablesList::clear() 
{ 
  Dict.clear(); 
}

// =====================================================
        
configParserStruct::structParserUtil::variablesListStack::variablesListStack() 
{ 
  clear(); 
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variablesListStack::~variablesListStack()
{
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variablesListStack::set( const std::string &Name, const variable &Var )
{
  assert( ! Stack.empty() );

  const std::string &GlobalName = globalName(Name);
  const std::string &LocalName = localName(Name);

  if ( ! GlobalName.empty() )
  {
    Stack.front().set( GlobalName, Var );
    return;
  }

  if ( ! LocalName.empty() )
  {
    Stack.back().set( LocalName, Var );
    return;
  }

  Stack.back().set( Name, Var );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesListStack::get( const std::string &Name ) const
{
  const variable *Variable = const_cast< variablesListStack* >(this)->getPointer( Name );
  if ( Variable == NULL )
    return variable();
  return *Variable;
}

// -----------------------------------------------------

configParserStruct::structParserUtil::variable* configParserStruct::structParserUtil::variablesListStack::getPointer( const std::string &Name )
{
  assert( ! Stack.empty() );
  
  const std::string &GlobalName = globalName(Name);
  const std::string &LocalName = localName(Name);

  if ( ! GlobalName.empty() )
  {
    return Stack.front().getPointer( GlobalName );
  }

  if ( ! LocalName.empty() )
  {
    return Stack.back().getPointer( LocalName );
  }

  for ( varListStack::reverse_iterator s = Stack.rbegin(); s != Stack.rend(); ++s )
  {
    variable *Variable = s->getPointer( Name );
    if ( Variable != NULL )
      return Variable;
  }
  return NULL;
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesListStack::getFromTopOfStack( const std::string &Name ) const 
{ 
  assert( ! Stack.empty() );
  return Stack.back().get(Name); 
}

// -----------------------------------------------------
        
const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesListStack::getFromLastByOneOfStack( const std::string &Name ) const
{
  assert( ! Stack.empty() );
  varListStack::const_iterator Iterator = Stack.end();
  --Iterator;
  if ( Iterator == Stack.begin() )
    return variable();
  --Iterator;
  return Iterator->get(Name);
}

// -----------------------------------------------------
        
const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variablesListStack::getFromStack( size_t Index, const std::string &Name ) const
{
  varListStack::const_iterator Iterator = Stack.begin();
  for ( size_t i = 0; i < Index && Iterator != Stack.end(); ++i )
    ++Iterator;

  if ( Iterator == Stack.end() )
    return variable();

  return Iterator->get(Name);
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variablesListStack::removeFromTopOfStack( const std::string &Name )
{
  assert( ! Stack.empty() );
  Stack.back().remove(Name); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::variablesListStack::pushNewList() 
{ 
  Stack.push_back( variablesList() ); 
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

std::set<std::string> configParserStruct::structParserUtil::variablesListStack::listOfNames() const
{
  std::set<std::string> Result;
  for ( varListStack::const_iterator s = Stack.begin(); s != Stack.end(); ++s )
  {
    const std::set<std::string> &Names = s->listOfNames();
    Result.insert( Names.begin(), Names.end() );
  }
  return Result;
}

// -----------------------------------------------------

std::set<std::string> configParserStruct::structParserUtil::variablesListStack::listOfNamesInAllStack() const
{
  std::set<std::string> Result;
  for ( varListStack::const_iterator s = Stack.begin(); s != Stack.end(); ++s )
  {
    const std::set<std::string> &Names = s->listOfNamesIncludeSubdict();
    Result.insert( Names.begin(), Names.end() );
  }
  return Result;
}

// -----------------------------------------------------

std::set<std::string> configParserStruct::structParserUtil::variablesListStack::listOfNamesInStack( size_t Index ) const
{
  varListStack::const_iterator Iterator = Stack.begin();
  for ( size_t i = 0; i < Index && Iterator != Stack.end(); ++i )
    ++Iterator;

  if ( Iterator == Stack.end() )
    return std::set<std::string>();
  return Iterator->listOfNames();
}

// -----------------------------------------------------

size_t configParserStruct::structParserUtil::variablesListStack::size() const 
{ 
  return Stack.size(); 
}

// -----------------------------------------------------

std::string configParserStruct::structParserUtil::variablesListStack::globalPrefix() 
{ 
  return "::"; 
}

// -----------------------------------------------------

std::string configParserStruct::structParserUtil::variablesListStack::globalName( const std::string &Name )
{
  const std::string &GlobalPrefix = globalPrefix();
  size_t Pos = Name.find( GlobalPrefix );
  if ( Pos == 0 )
    return Name.substr( GlobalPrefix.length(), std::string::npos );
  return std::string();
}

// -----------------------------------------------------
        
std::string configParserStruct::structParserUtil::variablesListStack::localPrefix()
{
  return "@@";
}

// -----------------------------------------------------

std::string configParserStruct::structParserUtil::variablesListStack::localName( const std::string &Name )
{
  const std::string &LocalPrefix = localPrefix();
  size_t Pos = Name.find( LocalPrefix );
  if ( Pos == 0 )
    return Name.substr( LocalPrefix.length(), std::string::npos );
  return std::string();
}

// =====================================================


