
// =====================================================

#include "configParserStruct2/text.h"
#include "configParserStruct2/exception.h"

#include <limits>
#include <cstring>
#include <cstddef>

// =====================================================
      
const int configParserStruct::text::SuccesssErrorLine = -1;
const char configParserStruct::text::PlaceholderByte = 0x00;

// -----------------------------------------------------

configParserStruct::text::text() :
  ErrorLine( SuccesssErrorLine )
{
  Text.reserve( 64*sizeof(command) );
  Strings.reserve( 32 );
}

// -----------------------------------------------------

configParserStruct::text::~text()
{
  clear();
}

// -----------------------------------------------------

configParserStruct::text::text( const text &Other ) :
  ErrorLine( Other.ErrorLine )
{
}

// -----------------------------------------------------
      
void configParserStruct::text::clear()
{
  for ( size_t i = 0; i < Strings.size(); i++ )
    delete [] Strings[i];
  Strings.clear();

  for ( size_t i = 0; i < size(); i++ )
    commandPointer(i)->~command();
  Text.clear();
}

// -----------------------------------------------------

void configParserStruct::text::push( const command &Command )
{
  Text.resize( Text.size() + sizeof(command), '\0' );
  Command.clone( commandPointer( size()-1 ) );
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
  for ( ptrdiff_t i = static_cast<ptrdiff_t>(size()) - 1; i >= 0; i-- )
  {
    void *Pointer = commandPointer(i);
    
    for ( size_t j = 0; j < sizeof(command) && Pointer != NULL; j++ )
    {
      if ( static_cast<const char*>(Pointer)[j] != PlaceholderByte )
        Pointer = NULL;
    }

    if ( Pointer != NULL )
    {
      Command.clone( Pointer );
      return;
    }
  }

  throw exception( "Can not find placeholder" );
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

  return std::numeric_limits<size_t>::max();
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

  for ( size_t i = 0; i < Strings.size(); i++ )
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
  ErrorLine = Line;
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
  return ErrorLine;
}

// =====================================================

