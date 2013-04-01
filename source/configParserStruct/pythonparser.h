
#pragma once

// =====================================================

#include "configParserStruct/parser.h"

#include <map>

// =====================================================

namespace configParserStruct
{

  class pythonParser : public parser
  {
    private:
      static int NumberOfPythonParsers;

      void *Dictionary; // pointer to PyObject
      std::map< std::string, std::string > ExternalVariables;

    private:
      pythonParser( const pythonParser& );
      pythonParser& operator=( const pythonParser& );

    private:
      void clearDictionary();
      void recreateDictionary();
      void runString( const std::string &Program );
      void checkError();

      void* finalClassPyobject( const std::string &Name ) const; // return PyObject
      static std::string pyobjectString( void *Object ); // arg is pointer to Pyobject
      static std::list<std::string> splitName( const std::string &Name );

      static std::string pythonQuotes() { return "'''"; }
      static std::string quoteString( const std::string &String );
      static std::string dequoteString( const std::string &String );

    public:
      pythonParser( const std::string &ConfigText = std::string() );
      ~pythonParser();
      
      void setVariableValue( const std::string &VarName, const std::string &Value );
      void setVariableValue( const std::string &VarName, int Value );
      void setVariableValue( const std::string &VarName, double Value );

      void exec( const std::string &ConfigText );

      bool isVariableExist( const std::string &VarName ) const;
      
      std::vector< std::string > listOfVariables() const;
      std::vector< std::string > listOfVariablesStruct() const;

      std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const;
  };

  // =====================================================
  

}


