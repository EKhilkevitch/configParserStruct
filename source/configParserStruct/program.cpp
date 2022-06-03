
// =====================================================

#include "configParserStruct/program.h"
#include "configParserStruct/bisoncompiler.h"
#include "configParserStruct/lexinput.h"
#include "configParserStruct/exception.h"
#include "configParserStruct/mutex.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cstdio>

// =====================================================

extern "C" int CPSSPU_parse( void );
extern "C" void CPSSPU_clearBuffers( void );
extern "C" void CPSSPU_lexResetLineNumber( void );
extern "C" void CPSSPU_lexSetNewLineEnabled( int Enabled );
extern "C" size_t CPSSPU_lexCurrentLineNumber( void );

// =====================================================

namespace
{
  configParserStruct::mutex YaccParserMutex;
}

// =====================================================

configParserStruct::program::program() :
  Text( new text() ),
  TextRefCount( new int(1) )
{
}

// -----------------------------------------------------

configParserStruct::program::program( const program &Other ) :
  Text( Other.Text ),
  TextRefCount( Other.TextRefCount ),
  Memory( Other.Memory )
{
  *TextRefCount += 1;
}

// -----------------------------------------------------

configParserStruct::program& configParserStruct::program::operator=( const program &Other )
{
  if ( &Other == this )
    return *this;

  program Copy(Other);
  Copy.swap(*this);

  return *this;
}

// -----------------------------------------------------

configParserStruct::program::~program()
{
  *TextRefCount -= 1;
  if ( *TextRefCount <= 0 )
  {
    delete TextRefCount;
    delete Text;
  }
}

// -----------------------------------------------------
      
void configParserStruct::program::swap( program &Other )
{
  std::swap( Other.Text, Text );
  std::swap( Other.TextRefCount, TextRefCount );
  Memory.swap( Other.Memory );
}

// -----------------------------------------------------

void configParserStruct::program::clear()
{
  Memory.clear();
  Text->clear();
}

// -----------------------------------------------------
 
void configParserStruct::program::build( const std::string &SourceCode )
{
  Text->clear();

  mutexLocker Locker( &YaccParserMutex );

  CPSSPU_setTextForBison( Text );
  CPSSPU_setParserInputString( SourceCode.c_str() );
  CPSSPU_lexResetLineNumber();
  CPSSPU_lexSetNewLineEnabled( 0 );

  const int ParseResult = CPSSPU_parse();

  //std::cerr << "program::build: " << "ParseResult = " << ParseResult << ", ErrorLine = " << Text.errorLine() << std::endl;

  //for ( configParserStruct::text::const_iterator it = Text->begin(); it != Text->end(); ++it )
  {
   // std::cerr << std::setw(5) << it - Text.begin() << " " << *it << std::endl;
  }

  CPSSPU_setTextForBison( NULL );
  CPSSPU_clearParserInputString();
  CPSSPU_clearBuffers();
  
  if ( ParseResult != 0 || Text->isError() )
  {
    char ErrorString[ 128 ] = { '\0' };
    std::snprintf( ErrorString, sizeof(ErrorString)-1, "Parser error at line %i", Text->errorLine() );
    throw exception( ErrorString );
  }
}

// -----------------------------------------------------

void configParserStruct::program::run()
{
  Memory.clear();
  while ( ! Memory.isHalted() )
  {
    const size_t InstructionPointer = Memory.instructionPointer();
    const command &Command = (*Text)[InstructionPointer];
    Command.exec( &Memory );
    //std::cerr << std::endl << Command << std::endl << Memory.toDebugString() << std::endl;
  }
}

// -----------------------------------------------------
      
const configParserStruct::text& configParserStruct::program::programText() const
{
  return *Text;
}

// -----------------------------------------------------

const configParserStruct::memory& configParserStruct::program::programMemory() const
{
  return Memory;
}

// -----------------------------------------------------

configParserStruct::memory& configParserStruct::program::programMemory() 
{
  return Memory;
}

// =====================================================
      
