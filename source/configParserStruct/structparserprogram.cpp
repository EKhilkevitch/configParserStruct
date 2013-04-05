
// =====================================================

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserinput.h"
#include "configParserStruct/mutex.h"

extern "C" int strprs_parse();

// =====================================================

static configParserStruct::mutex Mutex;

// =====================================================

configParserStruct::structParser::program::program()
{
}

// -----------------------------------------------------

configParserStruct::structParser::program::~program()
{
}

// -----------------------------------------------------

void configParserStruct::structParser::program::clear()
{
  Commands.clear();
  Variables.clear();
  Stack.clear();
}

// -----------------------------------------------------
        
void configParserStruct::structParser::program::build( const std::string &ProgramText )
{
  clear();

  Mutex.lock();
  try
  {
    setStructPrserProgram( this );
    setInputString( ProgramText ); 
    
    int ParseResult = strprs_parse();

    setStructPrserProgram( NULL );
  } catch ( ... )
  {
    Mutex.unlock();
    throw;
  }
    
  Mutex.unlock();
}

// =====================================================

