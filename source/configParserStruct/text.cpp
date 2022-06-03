
// =====================================================

#include "configParserStruct/text.h"
#include "configParserStruct/exception.h"

#include <limits>
#include <cstring>
#include <cstddef>
#include <cassert>

// =====================================================
      
const int configParserStruct::text::SuccesssErrorLine = -1;
const char configParserStruct::text::PlaceholderByte = 0x00;
const size_t configParserStruct::text::InvalidPlaceholderIndex = std::numeric_limits<size_t>::max();

// -----------------------------------------------------

configParserStruct::text::text()
{
  Text.reserve( 64*sizeof(command) );
  Strings.reserve( 32 );

  ParseTimeStatus.ErrorLine = SuccesssErrorLine;
  ParseTimeStatus.FunctionLevel = 0;
}

// -----------------------------------------------------

configParserStruct::text::~text()
{
  clear();
}

// -----------------------------------------------------
      
void configParserStruct::text::clear()
{
  for ( size_t i = 0; i < Strings.size(); i++ )
    delete [] Strings[i];
  Strings.clear();

  const size_t TextSize = size();
  for ( size_t i = 0; i < TextSize; i++ )
    commandPointer(i)->~command();
  Text.clear();

  ParseTimeStatus.ErrorLine = SuccesssErrorLine;
  ParseTimeStatus.FunctionLevel = 0;
  ParseTimeStatus.Variables.clear();
}

// -----------------------------------------------------

void configParserStruct::text::push( const command &Command )
{
  Text.resize( Text.size() + sizeof(command), '\0' );

  try
  {
    Command.clone( commandPointer( size()-1 ) );
  } catch ( ... ) {
    Text.resize( Text.size() - sizeof(command) );
    throw;
  }
}

// -----------------------------------------------------
      
void configParserStruct::text::pushPlaceholder()
{
  Text.resize( Text.size() + sizeof(command), '\0' );
  std::memset( static_cast<void*>( commandPointer( size()-1 ) ), PlaceholderByte, sizeof(command) );
}

// -----------------------------------------------------

void configParserStruct::text::replacePlaceholder( const command &Command )
{
  const size_t IndexOfPlaceholder = indexOfPlaceholder();
  if ( IndexOfPlaceholder == InvalidPlaceholderIndex )
    throw exception( "Can not find placeholder" );

  void *Pointer = commandPointer(IndexOfPlaceholder);
  Command.clone( Pointer );
}

// -----------------------------------------------------
      
size_t configParserStruct::text::indexOfPlaceholder() const
{
  for ( ptrdiff_t i = static_cast<ptrdiff_t>(size()) - 1; i >= 0; i-- )
  {
    const void *Pointer = commandPointer(i);
    
    bool IsPlaceholder = true;
    for ( size_t j = 0; j < sizeof(command) && IsPlaceholder; j++ )
    {
      if ( static_cast<const char*>(Pointer)[j] != PlaceholderByte )
        IsPlaceholder = false;
    }

    if ( IsPlaceholder )
      return i;
  }

  return InvalidPlaceholderIndex;
}

// -----------------------------------------------------

void configParserStruct::text::replaceLastCommand( const command &Command )
{
  if ( empty() )
    return;

  command *Pointer = commandPointer( size() - 1 );
  Pointer->~command();

  Command.clone(Pointer);
}

// -----------------------------------------------------

const char* configParserStruct::text::constStringRef( const char *String )
{
  if ( String == NULL )
    return NULL;

  for ( ptrdiff_t i = static_cast<ptrdiff_t>(Strings.size()) - 1; i >= 0; i-- )
  {
    if ( std::strcmp( Strings[i], String ) == 0 )
      return Strings[i];
  }

  Strings.reserve( Strings.size() + 1 );
  Strings.push_back( new char[ std::strlen(String) + 1 ] );
  std::strcpy( Strings.back(), String );
  return Strings.back();
}

// -----------------------------------------------------
      
void configParserStruct::text::setErrorLine( int Line )
{
  ParseTimeStatus.ErrorLine = Line;
}

// -----------------------------------------------------

void configParserStruct::text::parseTimeIncFunctionLevel()
{
  ParseTimeStatus.FunctionLevel += 1;
}

// -----------------------------------------------------

void configParserStruct::text::parseTimeDecFunctionLevel()
{
  ParseTimeStatus.FunctionLevel -= 1;
}

// -----------------------------------------------------

void configParserStruct::text::addToVariablesSet( const char *Name, bool Force )
{
  assert( Name != NULL );

  if ( ParseTimeStatus.FunctionLevel > 0 && !Force )
    return;

  const char *FixedName = constStringRef(Name);
  ParseTimeStatus.Variables.insert( FixedName );
}

// -----------------------------------------------------
      
void configParserStruct::text::eraseFromVariablesSet( const char *Name )
{
  assert( Name != NULL );
  
  if ( ParseTimeStatus.FunctionLevel > 0 )
    return;

  const char *FixedName = constStringRef(Name);
  ParseTimeStatus.Variables.erase( FixedName );
}

// -----------------------------------------------------
      
configParserStruct::command* configParserStruct::text::commandPointer( size_t Index )
{
  void *BasePointer = static_cast<void*>( &Text[0] );
  return static_cast< command* >( BasePointer ) + Index;
}

// -----------------------------------------------------

const configParserStruct::command* configParserStruct::text::commandPointer( size_t Index ) const
{
  const void *BasePointer = static_cast<const void*>( &Text[0] );
  return static_cast< const command* >( BasePointer ) + Index;
}

// -----------------------------------------------------
      
const configParserStruct::command& configParserStruct::text::operator[]( size_t Index ) const
{
  if ( Index >= size() )
    throw exception( "Instruction index out of ranges" );

  return *commandPointer(Index);
}

// -----------------------------------------------------

configParserStruct::text::const_iterator configParserStruct::text::begin() const
{
  return commandPointer(0);
}

// -----------------------------------------------------

configParserStruct::text::const_iterator configParserStruct::text::end() const
{
  return commandPointer( size() );
}

// -----------------------------------------------------

size_t configParserStruct::text::size() const
{
  return Text.size() / sizeof(command);
}

// -----------------------------------------------------
      
bool configParserStruct::text::empty() const
{
  return Text.empty();
}

// -----------------------------------------------------
      
bool configParserStruct::text::isError() const
{
  return errorLine() != SuccesssErrorLine;
}

// -----------------------------------------------------

int configParserStruct::text::errorLine() const
{
  return ParseTimeStatus.ErrorLine;
}

// -----------------------------------------------------
      
std::set<std::string> configParserStruct::text::variables() const
{
  std::set<std::string> Result;

  for ( std::set< const char* >::const_iterator it = ParseTimeStatus.Variables.begin(); it != ParseTimeStatus.Variables.end(); ++it )
    Result.insert( *it );

  return Result;
}

// =====================================================

