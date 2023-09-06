
// =====================================================

#include "configParserStruct/variable.h"
#include "configParserStruct/variablevalue.h"
#include "configParserStruct/reference.h"

#include <limits>
#include <ostream>
#include <cassert>

// =====================================================

namespace
{
  template <bool S> struct static_assert_test { char Array[ S ? 1 : -1 ]; };
}

// =====================================================

configParserStruct::variable::variable() 
{
  static_assert_test< sizeof(ValueMemory) >= sizeof(undefVariableValue) >();

  undefVariableValue *Memory = variableValuePointer<undefVariableValue>();
  new (Memory) undefVariableValue();
}

// -----------------------------------------------------

configParserStruct::variable::variable( int Integer ) 
{
  static_assert_test< sizeof(ValueMemory) >= sizeof(integerVariableValue) >();

  integerVariableValue *Memory = variableValuePointer<integerVariableValue>();
  new (Memory) integerVariableValue(Integer);
}

// -----------------------------------------------------

configParserStruct::variable::variable( double Real ) 
{
  static_assert_test< sizeof(ValueMemory) >= sizeof(realVariableValue) >();

  realVariableValue *Memory = variableValuePointer<realVariableValue>();
  new (Memory) realVariableValue(Real);
}

// -----------------------------------------------------

configParserStruct::variable::variable( const char *String ) 
{
  static_assert_test< sizeof(ValueMemory) >= sizeof(stringVariableValue) >();

  stringVariableValue *Memory = variableValuePointer<stringVariableValue>();
  new (Memory) stringVariableValue(String);
}

// -----------------------------------------------------

configParserStruct::variable::variable( const std::string &String ) 
{
  static_assert_test< sizeof(ValueMemory) >= sizeof(stringVariableValue) >();

  stringVariableValue *Memory = variableValuePointer<stringVariableValue>();
  new (Memory) stringVariableValue(String.c_str());
}

// -----------------------------------------------------
      
configParserStruct::variable::variable( const reference &Reference )
{
  static_assert_test< sizeof(ValueMemory) >= sizeof(referenceVariableValue) >();
      
  void *Memory = variableValuePointer<variableValue>();
  new (Memory) referenceVariableValue(Reference);
}

// -----------------------------------------------------
      
configParserStruct::variable::variable( const collectionType Type )
{
  void *Memory = variableValuePointer<variableValue>();

  switch ( Type )
  {
    case ArrayCollection:
      static_assert_test< sizeof(ValueMemory) >= sizeof(arrayVariableValue) >();
      new (Memory) arrayVariableValue();
      break;
    
    case DictCollection:
      static_assert_test< sizeof(ValueMemory) >= sizeof(dictVariableValue) >();
      new (Memory) dictVariableValue();
      break;

    default:
      assert( false );
      new (Memory) undefVariableValue();
      break;
  }
}

// -----------------------------------------------------
      
configParserStruct::variable::variable( const variable &Variable ) 
{
  const variableValue *OtherValue = Variable.variableValuePointer<variableValue>();
  variableValue *CurrentMemory = variableValuePointer<variableValue>();
  OtherValue->clone( CurrentMemory );
}

// -----------------------------------------------------

configParserStruct::variable& configParserStruct::variable::operator=( const variable &Variable )
{
  if ( &Variable == this )
    return *this;

#if 0
  variable Other( Variable );
  Other.swap( *this );
  return *this;
#endif

  const variableValue *OtherValue = Variable.variableValuePointer<variableValue>();
  variableValue *ThisValue = variableValuePointer<variableValue>();

  ThisValue->~variableValue();

  try
  {
    OtherValue->clone(ThisValue);
  } catch ( ... ) {
    new (ThisValue) undefVariableValue();
    throw;
  }
  return *this;
}

// -----------------------------------------------------

configParserStruct::variable::~variable() 
{
  variableValuePointer<variableValue>()->~variableValue();
}

// -----------------------------------------------------
      
template < class T > T* configParserStruct::variable::variableValuePointer()
{
  void *VoidMemory = static_cast<void*>( &ValueMemory );
  return static_cast< T* >( VoidMemory );
}

// -----------------------------------------------------

template < class T > const T* configParserStruct::variable::variableValuePointer() const
{
  const void *VoidMemory = static_cast<const void*>( &ValueMemory );
  return static_cast< const T* >( VoidMemory );
}

// -----------------------------------------------------

void configParserStruct::variable::swap( variable &Other )
{
  valueMemory Tmp = Other.ValueMemory;
  Other.ValueMemory = ValueMemory;
  ValueMemory = Tmp;
}

// -----------------------------------------------------

int configParserStruct::variable::integer() const
{
  return variableValuePointer<variableValue>()->integer();
}

// -----------------------------------------------------

double configParserStruct::variable::real() const
{
  return variableValuePointer<variableValue>()->real();
}

// -----------------------------------------------------

bool configParserStruct::variable::boolean() const
{
  return variableValuePointer<variableValue>()->boolean();
}

// -----------------------------------------------------

std::string configParserStruct::variable::string() const
{
  return variableValuePointer<variableValue>()->string();
}

// -----------------------------------------------------

const configParserStruct::reference& configParserStruct::variable::ref() const
{
  return variableValuePointer<variableValue>()->ref();
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::variable::getByRef( const reference &Reference ) const
{
  return variableValuePointer<variableValue>()->getByRef( Reference );
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::variable::setByRef( const reference &Reference, const variable &Variable )
{
  return variableValuePointer<variableValue>()->setByRef( Reference, Variable );
}

// -----------------------------------------------------
      
const char* configParserStruct::variable::type() const
{
  return variableValuePointer<variableValue>()->type();
}

// =====================================================
  
std::ostream& configParserStruct::operator<<( std::ostream &Stream, const variable &Variable )
{
  return Stream << Variable.type() << ":" << Variable.string();
}

// =====================================================

