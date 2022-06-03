
// =====================================================

#include "configParserStruct2/program.h"
#include "configParserStruct2/bisoncompiler.h"
#include "configParserStruct2/lexinput.h"
#include "configParserStruct2/exception.h"
#include "configParserStruct2/mutex.h"

#include <cstdio>
#include <iomanip>
#include <iostream>

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

configParserStruct::program::program()
{
}

// -----------------------------------------------------

configParserStruct::program::~program()
{
}

// -----------------------------------------------------

void configParserStruct::program::clear()
{
  Memory.clear();
  Text.clear();
}

// -----------------------------------------------------
 
void configParserStruct::program::build( const std::string &SourceCode )
{
  Text.clear();

  mutexLocker Locker( &YaccParserMutex );

  CPSSPU_setTextForBison( &Text );
  CPSSPU_setParserInputString( SourceCode.c_str() );
  CPSSPU_lexResetLineNumber();
  CPSSPU_lexSetNewLineEnabled( 0 );

  const int ParseResult = CPSSPU_parse();

  //std::cerr << "program::build: " << "ParseResult = " << ParseResult << ", ErrorLine = " << Text.errorLine() << std::endl;

  for ( configParserStruct::text::const_iterator it = Text.begin(); it != Text.end(); ++it )
  {
   // std::cerr << std::setw(5) << it - Text.begin() << " " << *it << std::endl;
  }

  CPSSPU_setTextForBison( NULL );
  CPSSPU_clearParserInputString();
  CPSSPU_clearBuffers();
  
  if ( ParseResult != 0 || Text.isError() )
  {
    char ErrorString[ 128 ] = { '\0' };
    std::snprintf( ErrorString, sizeof(ErrorString)-1, "Parser error at line %i", Text.errorLine() );
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
    const command &Command = Text[InstructionPointer];
    Command.exec( &Memory );
    //std::cerr << std::endl << Command << std::endl << Memory.toDebugString() << std::endl;
  }
}

// -----------------------------------------------------
      
const configParserStruct::text& configParserStruct::program::programText() const
{
  return Text;
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
      
