
// =====================================================

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserinput.h"
#include "configParserStruct/mutex.h"

extern "C" int strprs_parse();

// =====================================================

static configParserStruct::mutex Mutex;

// =====================================================

configParserStruct::structParserUtil::program::program()
{
}

// -----------------------------------------------------

configParserStruct::structParserUtil::program::~program()
{
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::program::clear()
{
  Commands.clear();
  Variables.clear();
  Stack.clear();
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::program::build( const std::string &ProgramText )
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

