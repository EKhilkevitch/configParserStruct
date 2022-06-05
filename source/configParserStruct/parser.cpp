
// =====================================================

#include "configParserStruct/parser.h"
#include "configParserStruct/program.h"
#include "configParserStruct/variable.h"
#include "configParserStruct/variablevalue.h"
#include "configParserStruct/reference.h"
#include "configParserStruct/exception.h"

#include <stdexcept>
#include <cassert>
#include <cstring>

// =====================================================
      
const char *const configParserStruct::parser::LastExpressionValueName = "..LEXP..";
const int configParserStruct::parser::SuccesssErrorLine = -1;

// -----------------------------------------------------

configParserStruct::parser::parser() :
  Program( new program() )
{
}

// -----------------------------------------------------
      
configParserStruct::parser::parser( const parser &Other ) :
  Program( new program(*Other.Program) )
{
}

// -----------------------------------------------------

configParserStruct::parser& configParserStruct::parser::operator=( const parser &Other )
{
  if ( &Other == this )
    return *this;

  parser Copy(Other);
  swap( Copy );

  return *this;
}

// -----------------------------------------------------

configParserStruct::parser::~parser()
{
  delete Program;
}

// -----------------------------------------------------
      
void configParserStruct::parser::swap( parser &Other )
{
  program *Tmp = Other.Program;
  Other.Program = Program;
  Program = Tmp;
}

// -----------------------------------------------------

void configParserStruct::parser::exec( const std::string &Text )
{
  build( Text );
  run();
}

// -----------------------------------------------------

void configParserStruct::parser::build( const std::string &Text )
{
  Program->build(Text);
}

// -----------------------------------------------------

void configParserStruct::parser::run()
{
  Program->run();
}

// -----------------------------------------------------

void configParserStruct::parser::reset()
{
  Program->clear();
}

// -----------------------------------------------------
      
bool configParserStruct::parser::isVariableExist( const std::string &Name ) const
{
  const variable *Variable = findVariable(Name);
  if ( Variable == NULL )
    return false;
  else
    return true;
}

// -----------------------------------------------------
      
enum configParserStruct::parser::variableType configParserStruct::parser::variableType( const std::string &Name ) const
{
  const variable *Variable = findVariable(Name);

  if ( Variable == NULL )
    return VarNone;

  const char *Type = Variable->type();
  if ( Type == undefVariableValue::TypeName )
    return VarUndef;
  if ( Type == integerVariableValue::TypeName )
    return VarInteger;
  if ( Type == realVariableValue::TypeName )
    return VarReal;
  if ( Type == stringVariableValue::TypeName )
    return VarString;
  if ( Type == arrayVariableValue::TypeName )
    return VarArray;
  if ( Type == dictVariableValue::TypeName )
    return VarDict;
  return VarOther;
}

// -----------------------------------------------------

std::string configParserStruct::parser::stringVariable( const std::string &Name, const std::string &DefaultValue ) const
{
  const variable *Variable = findVariable(Name);
  if ( Variable == NULL )
    return DefaultValue;
  return Variable->string();
}

// -----------------------------------------------------
      
double configParserStruct::parser::doubleVariable( const std::string &Name, double DefaultValue ) const
{
  const variable *Variable = findVariable(Name);
  if ( Variable == NULL )
    return DefaultValue;
  return Variable->real();
}

// -----------------------------------------------------

int configParserStruct::parser::integerVariable( const std::string &Name, int DefaultValue ) const
{
  const variable *Variable = findVariable(Name);
  if ( Variable == NULL )
    return DefaultValue;
  return Variable->integer();
}

// -----------------------------------------------------
      
const configParserStruct::variable* configParserStruct::parser::findVariable( const std::string &Name ) const
{
  if ( Name == LastExpressionValueName )
    return &Program->programMemory().lastResult();

  const variable *Result = Program->programMemory().findValueByName(Name,named::GlobalScope);
  if ( Result == NULL )
  {
    std::vector<const char*> ConstChars;
    const reference Reference = referenceForName(Name,&ConstChars);
    if ( Reference.hasType( reference::None ) )
    {
      freeConstChars(&ConstChars);
      return NULL;
    }
    
    Result = Program->programMemory().findValueByName( Reference.asGlobalName(), named::GlobalScope );
    const reference *Next = Reference.next();

    while ( Next != NULL && Result != NULL )
    {
      Result = Result->getByRef( *Next );
      Next = Next->next();
    }

    freeConstChars(&ConstChars);
    return Result;
  }

  return Result;
}

// -----------------------------------------------------
      
void configParserStruct::parser::freeConstChars( std::vector<const char*> *ConstChars )
{
  assert( ConstChars != NULL );

  for ( std::vector<const char*>::const_iterator it = ConstChars->begin(); it != ConstChars->end(); ++it )
    delete [] *it;
  ConstChars->clear();
}

// -----------------------------------------------------
      
const char* configParserStruct::parser::stringFromInterval( std::string::const_iterator Begin, std::string::const_iterator End )
{
  char *Result = new char[ End - Begin + 1 ];
  for ( std::string::const_iterator it = Begin; it != End; it++ )
    Result[ it - Begin ] = *it;
  Result[ End - Begin ] = '\0';
  return Result;
}

// -----------------------------------------------------
      
configParserStruct::reference configParserStruct::parser::referenceForName( const std::string &Name, std::vector<const char*> *ConstChars )
{
  assert( ConstChars != NULL );
  assert( Name != LastExpressionValueName );

  reference Reference;

  std::string::const_iterator Begin = Name.begin();
  for ( std::string::const_iterator it = Name.begin(); true; ++it )
  {
    if ( it == Name.end() || *it == '.' )
    {
      ConstChars->push_back( NULL );
      ConstChars->back() = stringFromInterval( Begin, it );
      if ( Reference.hasType(reference::None) )
        Reference = reference( ConstChars->back(), reference::GlobalName );
      else 
        Reference.setAsTail( reference( ConstChars->back(), reference::DictKey ) );
      Begin = it + 1;
    }

    if ( it == Name.end() )
      break;
  }

  return Reference;
}

// -----------------------------------------------------
      
void configParserStruct::parser::setVariable( const std::string &Name, const std::string &Value )
{
  setVariable( Name, variable(Value) );
}

// -----------------------------------------------------

void configParserStruct::parser::setVariable( const std::string &Name, int Value )
{
  setVariable( Name, variable(Value) );
}

// -----------------------------------------------------

void configParserStruct::parser::setVariable( const std::string &Name, double Value )
{
  setVariable( Name, variable(Value) );
}

// -----------------------------------------------------
      
void configParserStruct::parser::setVariable( const std::string &Name, const variable &Value )
{
  if ( Name == LastExpressionValueName )
    throw exception( "Can not explicit set last expression value" );

  Program->programMemory().setValueByName( Name, Value, named::PresetScope );
}

// -----------------------------------------------------
      
std::set<std::string> configParserStruct::parser::variables() const
{
  return Program->programText().variables();
}

// -----------------------------------------------------

int configParserStruct::parser::errorLine() const
{
  const int Line = Program->programText().errorLine();
  if ( Line == text::SuccesssErrorLine )
    return SuccesssErrorLine;
  return Line;
}

// =====================================================

