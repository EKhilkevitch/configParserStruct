
// =====================================================

#include "configParserStruct/pythonparser.h"
#include "configParserStruct/stringcast.h"

#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include <python2.6/Python.h>

// =====================================================

static inline PyObject* castToDictionary( void *Arg )
{
  return static_cast<PyObject*>(Arg);
}

// -----------------------------------------------------

int configParserStruct::pythonParser::NumberOfPythonParsers = 0;

// -----------------------------------------------------

configParserStruct::pythonParser::pythonParser( const std::string &ConfigText ) :
  Dictionary(NULL)
{
  if ( NumberOfPythonParsers <= 0 )
    Py_Initialize();
  NumberOfPythonParsers++;

  if ( ! ConfigText.empty() )
    exec( ConfigText );
}

// -----------------------------------------------------

configParserStruct::pythonParser::~pythonParser()
{
  clearDictionary();

  NumberOfPythonParsers--;
  if ( NumberOfPythonParsers <= 0 )
    Py_Finalize();
}

// -----------------------------------------------------

void configParserStruct::pythonParser::exec( const std::string &ConfigText )
{
  recreateDictionary();
  runString( ConfigText );
  checkError();
}

// -----------------------------------------------------

void configParserStruct::pythonParser::recreateDictionary()
{
  clearDictionary();
  
  Dictionary = (void*)( PyDict_New() );
  PyDict_SetItemString( castToDictionary(Dictionary), "__builtins__", PyEval_GetBuiltins() );
}

// -----------------------------------------------------

void configParserStruct::pythonParser::runString( const std::string &Program )
{
  std::string PrefixString;
  for ( std::map<std::string,std::string>::const_iterator i = ExternalVariables.begin(); i != ExternalVariables.end(); ++i )
    PrefixString += i->first + " = " + i->second + "\n";

  PyRun_String( ( PrefixString + Program ).c_str(), Py_file_input, castToDictionary(Dictionary), castToDictionary(Dictionary) );
}

// -----------------------------------------------------

void configParserStruct::pythonParser::checkError()
{
  void *Error = PyErr_Occurred();
  if ( Error != NULL )
  {
    std::string ExceptionString;
    PyObject *ErrType, *ErrValue, *TraceBack;
    PyErr_Fetch( &ErrType, &ErrValue, &TraceBack );

    if ( ErrValue != NULL )
      ExceptionString = pyobjectString( ErrValue );

    Py_XDECREF(ErrType);
    Py_XDECREF(ErrValue);
    Py_XDECREF(TraceBack);

    throw std::runtime_error( ExceptionString );
  }
}

// -----------------------------------------------------

std::string configParserStruct::pythonParser::pyobjectString( void *Object )
{
  assert( Object != NULL );

  PyObject *PyStr = PyObject_Str( static_cast<PyObject*>(Object) );
  std::string String = PyString_AS_STRING( PyStr );
  Py_DECREF(PyStr);

  return String;
}

// -----------------------------------------------------

void configParserStruct::pythonParser::clearDictionary()
{
  if ( Dictionary != NULL )
  {
    Py_DECREF( castToDictionary(Dictionary) );
    Dictionary = NULL;
  }
}

// -----------------------------------------------------
      
bool configParserStruct::pythonParser::isVariableExist( const std::string &VarName ) const
{
  std::map< std::string, std::string >::const_iterator ExternalVarIterator = ExternalVariables.find( VarName );
  if ( ExternalVarIterator != ExternalVariables.end() )
    return true;
  return finalClassPyobject( VarName ) != NULL;
}

// -----------------------------------------------------

void* configParserStruct::pythonParser::finalClassPyobject( const std::string &Name ) const
{
  if ( Dictionary == NULL )
    return NULL;

  PyObject *Object = castToDictionary(Dictionary);
  const std::list<std::string> &NameFields = splitName(Name);

  for ( std::list<std::string>::const_iterator Field = NameFields.begin(); Field != NameFields.end(); ++Field )
  {
    Object = ( Object == Dictionary ? PyDict_GetItemString : PyObject_GetAttrString )( Object, Field->c_str() );
    //std::cout << "Field = " << *Field << " Object = " << Object << std::endl;
    if ( Object == NULL )
      break;
  }
  return Object;
}

// -----------------------------------------------------

std::list<std::string> configParserStruct::pythonParser::splitName( const std::string &Name )
{
  const char SplitChar = '.';

  std::list< std::string > List;

  size_t PrevPos = 0;
  while ( true )
  {
    size_t Pos = Name.find( SplitChar, PrevPos );
    if ( Pos == std::string::npos )
    {
      List.push_back( std::string( Name.begin()+PrevPos, Name.end() ) );
      return List;
    } else {
      List.push_back( std::string( Name.begin()+PrevPos, Name.begin() + Pos ) );
      PrevPos = Pos + 1;
    }
  }

}

// -----------------------------------------------------

std::vector< std::string > configParserStruct::pythonParser::listOfVariables() const
{
  return std::vector< std::string >();
}

// -----------------------------------------------------

std::vector< std::string > configParserStruct::pythonParser::listOfVariablesStruct() const
{
  return std::vector< std::string >();
}

// -----------------------------------------------------

std::string configParserStruct::pythonParser::stringVariable( const std::string &VarName, const std::string &DefaultValue ) const
{
  std::map< std::string, std::string >::const_iterator ExternalVarIterator = ExternalVariables.find( VarName );
  if ( ExternalVarIterator != ExternalVariables.end() )
    return dequoteString( ExternalVarIterator->second );
  
  if ( Dictionary == NULL )
    return DefaultValue;

  void *PResult = finalClassPyobject( VarName ); //PyDict_GetItemString( Dictionary, VarName.c_str() );
  if ( PResult == NULL )
    return DefaultValue;

  return pyobjectString( PResult );
}

// -----------------------------------------------------
      
void configParserStruct::pythonParser::setVariableValue( const std::string &VarName, const std::string &Value )
{
  ExternalVariables[ VarName ] = quoteString( Value );
}

// -----------------------------------------------------

void configParserStruct::pythonParser::setVariableValue( const std::string &VarName, int Value )
{
  ExternalVariables[ VarName ] = convertToString(Value);
}

// -----------------------------------------------------

void configParserStruct::pythonParser::setVariableValue( const std::string &VarName, double Value )
{
  ExternalVariables[ VarName ] = convertToString(Value);
}
      
// -----------------------------------------------------

std::string configParserStruct::pythonParser::quoteString( const std::string &String )
{
  return pythonQuotes() + String + pythonQuotes();
}

// -----------------------------------------------------

std::string configParserStruct::pythonParser::dequoteString( const std::string &String )
{
  const std::string &Quotes = pythonQuotes();
  size_t QuotesLength = Quotes.length();

  unsigned FirstQuotesIndex = String.find( Quotes );
  unsigned LastQuotesIndex = String.rfind( Quotes );

  if ( FirstQuotesIndex == 0 && LastQuotesIndex == String.length() - QuotesLength )
    return String.substr( QuotesLength, String.length() - 2*QuotesLength );

  return String;
}

// =====================================================

