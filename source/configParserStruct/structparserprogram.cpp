
// =====================================================

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserinput.h"
#include "configParserStruct/mutex.h"

#include <sstream>

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
        
// -----------------------------------------------------

void configParserStruct::structParserUtil::program::rebuildAndExecute( const std::string &ProgramText )
{
  clear();
  build( ProgramText );
  execute();
}

// -----------------------------------------------------
        
std::string configParserStruct::structParserUtil::program::toString() const
{
  std::stringstream Stream;

  Stream << "Program: " << Commands.size() << " commands, " << Variables.size() << " named variabled" << std::endl;

  Stream << std::endl << "Commands:" << std::endl;
  for ( unsigned i = 0; i < Commands.size(); i++ )
    Stream << "  " << Commands.getCommand(i).toString() << std::endl;
  
  Stream << std::endl << "Variables:" << std::endl;
  std::list<std::string> Names = Variables.listOfNames();
  for ( std::list<std::string>::const_iterator i = Names.begin(); i != Names.end(); ++i )
    Stream << "  " << *i << " = " << Variables.get( *i ).string() << std::endl;

  return Stream.str();
}

// =====================================================

