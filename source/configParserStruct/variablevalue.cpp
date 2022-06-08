
// =====================================================

#include "configParserStruct/variablevalue.h"
#include "configParserStruct/variable.h"
#include "configParserStruct/reference.h"
#include "configParserStruct/exception.h"

#include <limits>
#include <stdexcept>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// =====================================================

namespace
{
  const configParserStruct::reference NoneReference;
}

// =====================================================

const configParserStruct::reference& configParserStruct::variableValue::ref() const
{
  return NoneReference;
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::variableValue::getByRef( const reference & ) const
{
  return NULL;
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::variableValue::setByRef( const reference &, const variable & )
{
  return NULL;
}

// =====================================================

const char *const configParserStruct::undefVariableValue::TypeName = "undef";

// -----------------------------------------------------

configParserStruct::undefVariableValue::undefVariableValue()
{
}

// -----------------------------------------------------
      
configParserStruct::undefVariableValue* configParserStruct::undefVariableValue::clone( void *Memory ) const
{
  if ( Memory != NULL )
  {
    return new (Memory) undefVariableValue();
  } else {
    return new undefVariableValue();
  }
}

// -----------------------------------------------------

const char* configParserStruct::undefVariableValue::type() const
{
  return TypeName;
}

// -----------------------------------------------------

int configParserStruct::undefVariableValue::integer() const
{
  return 0;
}

// -----------------------------------------------------

double configParserStruct::undefVariableValue::real() const
{
  return 0;
}

// -----------------------------------------------------

bool configParserStruct::undefVariableValue::boolean() const
{
  return false;
}

// -----------------------------------------------------

std::string configParserStruct::undefVariableValue::string() const
{
  return std::string();
}

// =====================================================

const char *const configParserStruct::integerVariableValue::TypeName = "integer";

// -----------------------------------------------------

configParserStruct::integerVariableValue::integerVariableValue( int V ) :
  Value(V)
{
}
     
// -----------------------------------------------------

configParserStruct::integerVariableValue* configParserStruct::integerVariableValue::clone( void *Memory ) const
{
  if ( Memory != NULL )
  {
    return new (Memory) integerVariableValue(Value);
  } else {
    return new integerVariableValue(Value);
  }
}

// -----------------------------------------------------

const char* configParserStruct::integerVariableValue::type() const
{
  return TypeName;
}

// -----------------------------------------------------

int configParserStruct::integerVariableValue::integer() const
{
  return Value;
}

// -----------------------------------------------------

double configParserStruct::integerVariableValue::real() const
{
  return static_cast<double>(Value);
}

// -----------------------------------------------------

bool configParserStruct::integerVariableValue::boolean() const
{
  return ( Value == 0 ) ? false : true;
}

// -----------------------------------------------------

std::string configParserStruct::integerVariableValue::string() const
{
  char String[ 64 ];
  std::snprintf( String, sizeof(String)-1, "%i", Value );
  return String;
}

// =====================================================

const char *const configParserStruct::realVariableValue::TypeName = "real";

// -----------------------------------------------------

configParserStruct::realVariableValue::realVariableValue( double V ) :
  Value(V)
{
}
     
// -----------------------------------------------------

configParserStruct::realVariableValue* configParserStruct::realVariableValue::clone( void *Memory ) const
{
  if ( Memory != NULL )
  {
    return new (Memory) realVariableValue(Value);
  } else {
    return new realVariableValue(Value);
  }
}

// -----------------------------------------------------

const char* configParserStruct::realVariableValue::type() const
{
  return TypeName;
}

// -----------------------------------------------------

int configParserStruct::realVariableValue::integer() const
{
  return static_cast<int>(Value);
}

// -----------------------------------------------------

double configParserStruct::realVariableValue::real() const
{
  return Value;
}

// -----------------------------------------------------

bool configParserStruct::realVariableValue::boolean() const
{
  return ( Value == 0 ) ? false : true;
}

// -----------------------------------------------------

std::string configParserStruct::realVariableValue::string() const
{
  char String[ 64 ];
  std::snprintf( String, sizeof(String)-1, "%g", Value );
  return String;
}

// =====================================================

const char *const configParserStruct::stringVariableValue::TypeName = "string";

// -----------------------------------------------------

configParserStruct::stringVariableValue::stringVariableValue( const char *V )
{
  if ( V == NULL )
    throw std::invalid_argument( "Value must be not NULL" );

  Value = new char[ std::strlen(V) + 1 ];
  std::strcpy( Value, V );
}
     
// -----------------------------------------------------

configParserStruct::stringVariableValue::~stringVariableValue()
{
  delete [] Value;
}

// -----------------------------------------------------

configParserStruct::stringVariableValue* configParserStruct::stringVariableValue::clone( void *Memory ) const
{
  if ( Memory != NULL )
  {
    return new (Memory) stringVariableValue(Value);
  } else {
    return new stringVariableValue(Value);
  }
}

// -----------------------------------------------------

const char* configParserStruct::stringVariableValue::type() const
{
  return TypeName;
}

// -----------------------------------------------------

int configParserStruct::stringVariableValue::integer() const
{
  return std::atoi(Value);
}

// -----------------------------------------------------

double configParserStruct::stringVariableValue::real() const
{
  return std::atof(Value);
}

// -----------------------------------------------------

bool configParserStruct::stringVariableValue::boolean() const
{
  if ( Value[0] == '\0' )
    return false;
  return true;
}

// -----------------------------------------------------

std::string configParserStruct::stringVariableValue::string() const
{
  return Value;
}

// =====================================================

const char *const configParserStruct::referenceVariableValue::TypeName = "ref";

// -----------------------------------------------------

configParserStruct::referenceVariableValue::referenceVariableValue( const reference &Value ) :
  Reference( Value )
{
}

// -----------------------------------------------------
      
configParserStruct::referenceVariableValue::referenceVariableValue( const char *Value, reference::type Type ) :
  Reference( Value, Type )
{
}

// -----------------------------------------------------
      
configParserStruct::referenceVariableValue::referenceVariableValue( size_t Value, reference::type Type ) :
  Reference( Value, Type )
{
}

// -----------------------------------------------------

configParserStruct::referenceVariableValue* configParserStruct::referenceVariableValue::clone( void *Memory ) const
{
  if ( Memory != NULL )
  {
    return new (Memory) referenceVariableValue(Reference);
  } else {
    return new referenceVariableValue(Reference);
  }
}

// -----------------------------------------------------

const char* configParserStruct::referenceVariableValue::type() const
{
  return TypeName;
}

// -----------------------------------------------------

int configParserStruct::referenceVariableValue::integer() const
{
  return static_cast<int>( -1 );
}

// -----------------------------------------------------

double configParserStruct::referenceVariableValue::real() const
{
  return integer();
}

// -----------------------------------------------------

bool configParserStruct::referenceVariableValue::boolean() const
{
  return Reference.hasType(reference::None) ? false : true;
}

// -----------------------------------------------------

std::string configParserStruct::referenceVariableValue::string() const
{
  return Reference.toDebugString();
}

// -----------------------------------------------------
      
const configParserStruct::reference& configParserStruct::referenceVariableValue::ref() const
{
  return Reference;
}

// =====================================================

const char *const configParserStruct::arrayVariableValue::TypeName = "array";

// -----------------------------------------------------

configParserStruct::arrayVariableValue::arrayVariableValue() 
{
}

// -----------------------------------------------------
      
configParserStruct::arrayVariableValue::arrayVariableValue( const std::vector< variable > &Values ) :
  Array( Values )
{
}

// -----------------------------------------------------

configParserStruct::arrayVariableValue::~arrayVariableValue()
{
}

// -----------------------------------------------------

configParserStruct::arrayVariableValue* configParserStruct::arrayVariableValue::clone( void *Memory ) const
{
  if ( Memory != NULL )
  {
    return new (Memory) arrayVariableValue(Array);
  } else {
    return new arrayVariableValue(Array);
  }
}

// -----------------------------------------------------

const char* configParserStruct::arrayVariableValue::type() const
{
  return TypeName;
}

// -----------------------------------------------------

int configParserStruct::arrayVariableValue::integer() const
{
  return static_cast<int>( Array.size() );
}

// -----------------------------------------------------

double configParserStruct::arrayVariableValue::real() const
{
  return static_cast<double>( integer() );
}

// -----------------------------------------------------

bool configParserStruct::arrayVariableValue::boolean() const
{
  return ! Array.empty();
}

// -----------------------------------------------------

std::string configParserStruct::arrayVariableValue::string() const
{
  std::ostringstream Stream;
  Stream << "[ ";
  for ( std::vector<variable>::const_iterator it = Array.begin(); it != Array.end(); ++it )
  {
    if ( it != Array.begin() )
      Stream << ", ";
    Stream << it->string();
  }
  Stream << " ]";
  return Stream.str();
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::arrayVariableValue::getByRef( const reference &Reference ) const
{
  const size_t Index = Reference.asArrayIndex();

  if ( Index >= Array.size() )
    Array.resize( Index + 1 );

  return &Array[Index];
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::arrayVariableValue::setByRef( const reference &Reference, const variable &Variable )
{
  size_t Index = Reference.asArrayIndex();

  if ( Index == std::numeric_limits<size_t>::max() )
    Index = Array.size();

  if ( Index >= Array.size() )
    Array.resize( Index + 1 );

  Array[Index] = Variable;
  return &Array[Index];
}

// =====================================================

const char *const configParserStruct::dictVariableValue::TypeName = "dict";
const char *const configParserStruct::dictVariableValue::ArrayOfKeysName = ".+!KEYS!+.";

// -----------------------------------------------------

configParserStruct::dictVariableValue::dictVariableValue() :
  Dict( new std::map< std::string, variable >() ),
  KeysArray( new variable( variable::ArrayCollection ) )
{
}

// -----------------------------------------------------
      
configParserStruct::dictVariableValue::dictVariableValue( const std::map< std::string, variable > &Values ) :
  Dict( new std::map< std::string, variable >(Values) ),
  KeysArray( new variable( variable::ArrayCollection ) )
{
}

// -----------------------------------------------------

configParserStruct::dictVariableValue::~dictVariableValue()
{
  delete Dict;
  delete KeysArray;
}

// -----------------------------------------------------

configParserStruct::dictVariableValue* configParserStruct::dictVariableValue::clone( void *Memory ) const
{
  if ( Memory != NULL )
  {
    return new (Memory) dictVariableValue(*Dict);
  } else {
    return new dictVariableValue(*Dict);
  }
}

// -----------------------------------------------------

const char* configParserStruct::dictVariableValue::type() const
{
  return TypeName;
}

// -----------------------------------------------------

int configParserStruct::dictVariableValue::integer() const
{
  return static_cast<int>( Dict->size() );
}

// -----------------------------------------------------

double configParserStruct::dictVariableValue::real() const
{
  return static_cast<double>( integer() );
}

// -----------------------------------------------------

bool configParserStruct::dictVariableValue::boolean() const
{
  return ! Dict->empty();
}

// -----------------------------------------------------

std::string configParserStruct::dictVariableValue::string() const
{
  std::ostringstream Stream;
  Stream << "{ ";

  for ( std::map< std::string, variable >::const_iterator it = Dict->begin(); it != Dict->end(); ++it )
  {
    if ( it != Dict->begin() )
      Stream << ", ";
    Stream << '.' << it->first << " = " << it->second.string();
  }

  Stream << " }";
  return Stream.str();
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::dictVariableValue::getByRef( const reference &Reference ) const
{
  const char *Key = Reference.asDictKey();

  if ( std::strcmp( Key, ArrayOfKeysName ) == 0 )
  {
    updateKeysArray();
    return KeysArray;
  }

  return &(*Dict)[Key];
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::dictVariableValue::setByRef( const reference &Reference, const variable &Variable )
{
  const char *Key = Reference.asDictKey();

  return &( (*Dict)[Key] = Variable );
}

// -----------------------------------------------------
      
void configParserStruct::dictVariableValue::updateKeysArray() const
{
  *KeysArray = variable( variable::ArrayCollection );

  for ( std::map< std::string, variable >::const_iterator it = Dict->begin(); it != Dict->end(); ++it )
    KeysArray->setByRef( reference( std::numeric_limits<size_t>::max(), reference::ArrayIndex ), variable( it->first.c_str() ) );
}

// =====================================================

