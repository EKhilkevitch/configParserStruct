
// =====================================================

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparserprogram.h"

#include <typeinfo>
#include <set>

// =====================================================

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variableValue::getValueByReference( const variable &Reference, unsigned AttrLevel ) const
{
  if ( AttrLevel == Reference.value<referenceVariableValue>().numberOfAttributes() )
    return *this;
  return variable();
}

// =====================================================

bool configParserStruct::structParserUtil::variable::isDefined() const 
{ 
  return typeid(*Value) != typeid(undefVariableValue); 
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::variable::getValueByReference( const variable &Reference, unsigned AttrLevel ) const
{
  return Value->getValueByReference(Reference,AttrLevel); 
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

  if ( isValueDerivedFrom<composerVariableValue>() && AttrLevel < RefValue.numberOfAttributes() )
  {
    value<composerVariableValue>().setValueByReference( Reference, Value, AttrLevel + 1 );
    return;
  }
    
  *this = undefVariableValue();
}

// =====================================================

unsigned configParserStruct::structParserUtil::builtinFunctionValue::getNumberOfArguments( const program &Program ) const
{
  return Program.getNamedVariableFromTopOfStack( "$N" ).integer();
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::builtinFunctionValue::getArgument( unsigned Index, const program &Program ) const
{
  std::string Name = "$" + convertToString(Index);
  return Program.getNamedVariableFromTopOfStack( Name );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::builtinFunctionValue::getVariable( const std::string &Name, const program &Program ) const
{
  return Program.getNamedVariable( Name );
}

// =====================================================

const std::string configParserStruct::structParserUtil::referenceVariableValue::string() const
{
  std::string Result;
  Result += Name;
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
  return Program.getNamedVariable( Name ).getValueByReference( *this );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::referenceVariableValue::setValue( program *Program, const variable &Value ) const
{
  assert( Program != NULL );

  variable *const Variable = Program->getNamedVariablePointer( Name );
  if ( Variable == NULL )
  {
    Program->setNamedVariable( Name, Value );
  } else {
    Variable->setValueByReference( *this, Value );
  }
}

// =====================================================
        
const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::composerVariableValue::getValueByReference( const variable &Reference, unsigned AttrLevel ) const
{
  const referenceVariableValue &RefValue = Reference.value<referenceVariableValue>();

  if ( AttrLevel > RefValue.numberOfAttributes() )
    return variable();

  if ( AttrLevel == RefValue.numberOfAttributes() )
    return *this;

  const variable &Key = RefValue.attribute( AttrLevel );
  return getItemByVariableKey( Key ).getValueByReference( Reference, AttrLevel + 1 );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::composerVariableValue::setValueByReference( const variable &Reference, const variable &Value, unsigned AttrLevel )
{
  const referenceVariableValue &RefValue = Reference.value<referenceVariableValue>();

  assert( AttrLevel < RefValue.numberOfAttributes() );

  const variable &Key = RefValue.attribute( AttrLevel );
  variable *Item = getItemPointerByVariableKey( Key );
  if ( Item == NULL )
  {
    addItemByVariableKey( Key, Value );
  } else {
    Item->setValueByReference( Reference, Value, AttrLevel + 1 );
  }
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

void configParserStruct::structParserUtil::arrayVariableValue::pushItem( const variable &Value )
{
  Array.push_back( Value );
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
  if ( Index >= Array.size() )
    return NULL;
  return &Array[Index];
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

  if ( GlobalName.empty() )
  {
    for ( varListStack::reverse_iterator s = Stack.rbegin(); s != Stack.rend(); ++s )
    {
      variable *Variable = s->getPointer( Name );
      if ( Variable != NULL )
        return Variable;
    }
    return NULL;
  } else {
    return Stack.front().getPointer( GlobalName );
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


