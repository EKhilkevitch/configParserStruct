
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
    std::vector<char> NameChars( Name.begin(), Name.end() );
    const reference Reference = referenceForName( &NameChars );
    if ( Reference.hasType( reference::None ) )
      return NULL;
    
    Result = Program->programMemory().findValueByName( Reference.asGlobalName(), named::GlobalScope );
    const reference *Next = Reference.next();

    while ( Next != NULL && Result != NULL )
    {
      Result = Result->getByRef( *Next );
      Next = Next->next();
    }

    if ( Result == NULL )
      return NULL;
    if ( Result->type() == undefVariableValue::TypeName )
      return NULL;
    return Result;
  }

  return Result;
}

// -----------------------------------------------------
      
configParserStruct::reference configParserStruct::parser::referenceForName( std::vector<char> *NameChars )
{
  assert( NameChars != NULL );

  if ( NameChars->empty() || NameChars->back() != '\0' )
    NameChars->push_back( '\0' );

  assert( std::strcmp( &(*NameChars)[0], LastExpressionValueName ) != 0 );

  reference Reference;

  char *Begin = &(*NameChars)[0];
  char *Current = Begin;
  while ( true )
  {
    bool IsLastChar = ( *Current == '\0' );
    if ( *Current == '\0' || *Current == '.' )
    {
      *Current = '\0';
      if ( Reference.hasType(reference::None) )
        Reference = reference( Begin, reference::GlobalName );
      else 
        Reference.setAsTail( reference( Begin, reference::DictKey ) );
      Begin = Current + 1;
    }

    if ( IsLastChar )
      break;

    ++Current;
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
      
std::set<std::string> configParserStruct::parser::dictKeys( const std::string &Name ) const
{
  const variable *Variable = findVariable(Name);

  if ( Variable == NULL || Variable->type() != dictVariableValue::TypeName )
    return std::set<std::string>();

  const variable *KeysArray = Variable->getByRef( reference( dictVariableValue::ArrayOfKeysName, reference::DictKey ) );
  assert( KeysArray != NULL );
  
  std::set<std::string> Result;

  const int KeysArraySize = KeysArray->integer();
  for ( int i = 0; i < KeysArraySize; i++ )
  {
    const variable *Key = KeysArray->getByRef( reference( static_cast<size_t>(i), reference::ArrayIndex ) );
    assert( Key != NULL );
    Result.insert( Key->string() );
  }

  return Result;
}

// -----------------------------------------------------

int configParserStruct::parser::errorLine() const
{
  const int Line = Program->programText().errorLine();
  if ( Line == text::SuccesssErrorLine )
    return SuccesssErrorLine;
  return Line;
}

// -----------------------------------------------------
      
std::string configParserStruct::parser::toDebugString() const
{
  return Program->programText().toDebugString() + '\n' +
         Program->programMemory().toDebugString() + '\n';
}

// =====================================================

