
// =====================================================

#include "configParserStruct/structparsercompiler.h"

#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>

// =====================================================

static configParserStruct::structParser::program *Program = NULL;

// =====================================================

void popValueFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( new configParserStruct::structParser::popCommand() );
}

// -----------------------------------------------------

void pushRealNumberToStack( double Number )
{
  configParserStruct::structParser::realVariableValue Value( Number );
  configParserStruct::structParser::variable Variable( Value );
  if ( Program != NULL )
    Program->pushCommand( new configParserStruct::structParser::pushCommand( Variable ) );
}

// -----------------------------------------------------

void pushIntegerNumberToStack( int Number )
{
  pushRealNumberToStack( Number );
}

// -----------------------------------------------------

void addValuesFromStack()
{
  if ( Program != NULL )
    Program->pushCommand( new configParserStruct::structParser::addCommand() );
}

// =====================================================

configParserStruct::structParser::command::command() :
  Action(NULL)
{
  Action = new nopCommand();
}

// -----------------------------------------------------

configParserStruct::structParser::command::command( const commandAction &A ) :
  Action(NULL)
{
  Action = A.clone();
  assert( Action != NULL );
}

// -----------------------------------------------------

configParserStruct::structParser::command::command( const command &C ) :
  Action(NULL)
{
  *this = C;
}

// -----------------------------------------------------

configParserStruct::structParser::command& configParserStruct::structParser::command::operator=( const command &C )
{
  if ( this != &C )
  {
    delete Action;
    Action = C.Action->clone();
  }
  assert( Action != NULL );
  return *this;
}

// -----------------------------------------------------

configParserStruct::structParser::command::~command()
{
  delete Action;
}

// =====================================================

configParserStruct::structParser::variable::variable() :
  Value(NULL)
{
  Value = new undefVariableValue();
}

// -----------------------------------------------------

configParserStruct::structParser::variable::variable( const variableValue &V, const std::string &N ) :
  Name(N),
  Value(NULL)
{
  Value = V.clone();
  assert( Value != NULL );
}

// -----------------------------------------------------

configParserStruct::structParser::variable::variable( const variable& V ) :
  Value(NULL)
{
  *this = V;
}

// -----------------------------------------------------

configParserStruct::structParser::variable& configParserStruct::structParser::variable::operator=( const variable& V )
{
  if ( &V != this )
  {
    delete Value;
    this->Name = V.Name;
    this->Value = V.Value->clone();
  }
    
  assert( Value != NULL );

  return *this;
}

// -----------------------------------------------------

configParserStruct::structParser::variable::~variable()
{
  delete Value;
}

// =====================================================

