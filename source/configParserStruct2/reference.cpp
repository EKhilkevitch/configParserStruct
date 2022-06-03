
// =====================================================

#include "configParserStruct2/reference.h"
#include "configParserStruct2/exception.h"

#include <algorithm>
#include <sstream>
#include <cstring>
#include <cassert>

// =====================================================

configParserStruct::reference::reference() :
  Type(None),
  Next(NULL)
{
}

// -----------------------------------------------------

configParserStruct::reference::reference( const char *V, type T ) :
  Type(T),
  Next(NULL)
{
  assert( Type == LocalName  || 
          Type == GlobalName ||
          Type == DictKey );

  if ( Type == DictKey )
  {
    if ( V == NULL )
      V = "";
    char *Copy = new char[ std::strlen(V) + 1 ];
    std::strcpy( Copy, V );
    Value.ConstChar = Copy;
  } else {
    Value.ConstChar = V;  
  }
}

// -----------------------------------------------------

configParserStruct::reference::reference( size_t V, type T ) :
  Type(T),
  Next(NULL)
{
  assert( Type == ArrayIndex || 
          Type == ArgumentIndex || 
          Type == InstructionPointer || 
          Type == StackPointer );

  Value.SizeT = V;
}

// -----------------------------------------------------
      
configParserStruct::reference::reference( const reference &Reference ) :
  Type( Reference.Type ),
  Value( Reference.Value ),
  Next(NULL)
{
  if ( Type == DictKey )
  {
    char *Copy = new char[ std::strlen(Reference.Value.ConstChar) + 1 ];
    std::strcpy( Copy, Reference.Value.ConstChar );
    Value.ConstChar = Copy;
  }

  if ( Reference.Next != NULL )
  {
    try
    {
      Next = new reference( *Reference.Next );
    } catch ( ... ) {
      delete [] Value.ConstChar;
      throw;
    }
  }
}

// -----------------------------------------------------

configParserStruct::reference& configParserStruct::reference::operator=( const reference &Reference )
{
  if ( &Reference == this )
    return *this;

  reference Copy(Reference);
  swap( Copy );

  return *this;
}

// -----------------------------------------------------

configParserStruct::reference::~reference()
{
  if ( Type == DictKey )
    delete [] Value.ConstChar;
  delete Next;
}

// -----------------------------------------------------
      
void configParserStruct::reference::swap( reference &Other )
{
  std::swap( Type,  Other.Type  );
  std::swap( Value, Other.Value );
  std::swap( Next,  Other.Next  );
} 

// -----------------------------------------------------
      
bool configParserStruct::reference::hasType( type T ) const
{
  if ( T == Type )
    return true;
  else
    return false;
}

// -----------------------------------------------------
      
const configParserStruct::reference* configParserStruct::reference::next() const
{
  return Next;
}

// -----------------------------------------------------
      
void configParserStruct::reference::setAsTail( const reference &Reference )
{
  if ( Next == NULL )
  {
    Next = new reference(Reference);
  } else {
    Next->setAsTail(Reference);
  }
}
      
// -----------------------------------------------------
      
const char* configParserStruct::reference::asGlobalName() const
{
  if ( Type != GlobalName )
    throw exception( "Invalid reference type (GlobalName)" );

  return Value.ConstChar;
}
      
// -----------------------------------------------------
      
const char* configParserStruct::reference::asLocalName() const
{
  if ( Type != LocalName )
    throw exception( "Invalid reference type (LocalName)" );

  return Value.ConstChar;
}

// -----------------------------------------------------
      
const char* configParserStruct::reference::asDictKey() const
{
  if ( Type != DictKey )
    throw exception( "Invalid reference type (DictKey)" );

  return Value.ConstChar;
}

// -----------------------------------------------------
      
size_t configParserStruct::reference::asArrayIndex() const
{
  if ( Type != ArrayIndex )
    throw exception( "Invalid reference type (ArrayIndex)" );

  return Value.SizeT;
}

// -----------------------------------------------------
      
size_t configParserStruct::reference::asArgumentIndex() const
{
  if ( Type != ArgumentIndex )
    throw exception( "Invalid reference type (ArgumentIndex)" );

  return Value.SizeT;
}

// -----------------------------------------------------
      
size_t configParserStruct::reference::asInstructionPointer() const
{
  if ( Type != InstructionPointer )
    throw exception( "Invalid reference type (InstructionPointer)" );

  return Value.SizeT;
}

// -----------------------------------------------------
      
size_t configParserStruct::reference::asStackPointer() const
{
  if ( Type != StackPointer )
    throw exception( "Invalid reference type (StackPointer)" );

  return Value.SizeT;
}

// -----------------------------------------------------
      
std::string configParserStruct::reference::toDebugString() const
{
  std::ostringstream Stream;

  switch ( Type )
  {
    case None:
      Stream << "(None)";
      break;

    case LocalName:
      Stream << "LocalName (" << Value.ConstChar << ")";
      break;

    case GlobalName:
      Stream << "GlobalName (" << Value.ConstChar << ")";
      break;
    
    case ArrayIndex:
      Stream << "ArrayIndex (" << Value.SizeT << ")";
      break;
    
    case DictKey:
      Stream << "DictKey (" << Value.ConstChar << ")";
      break;

    case ArgumentIndex:
      Stream << "ArgumentIndex (" << Value.SizeT << ")";
      break;

    case InstructionPointer:
      Stream << "InstructionPointer (" << Value.SizeT << ")";
      break;

    case StackPointer:
      Stream << "StackPointer (" << Value.SizeT << ")";
      break;

    default:
      Stream << "(Unknown)";
      break;
  }

  if ( Next != NULL )
    Stream << " -> " << Next->toDebugString();
  
  return Stream.str();
}

// =====================================================

