
#pragma once

// =====================================================

#include "configParserStruct/parser.h"
#include "configParserStruct/exception.h"

// =====================================================

namespace configParserStruct
{

  namespace structParserUtil { class program; }

  class structParser : public parser
  {
    public:
      struct exception : public ::configParserStruct::exception 
      {
        exception( const std::string &What );
      };

      class parsingException : public exception
      {
        private:
          int Line;
        public:
          parsingException( int Line );
          int line() { return Line; }
      };

    private:
      structParserUtil::program *Program;

    private:
      structParser( const structParser& );
      structParser& operator=( const structParser& );

    public:
      explicit structParser( const std::string &ConfigText = std::string() );
      ~structParser();

      void exec( const std::string &ConfigText );
      bool isVariableExist( const std::string &VarName ) const;

      void build( const std::string &ConfigText );
      void run();
      
      void reset();
      double lastDoubleVariable() const;
      
      containerForVariables listOfVariables() const;
      
      void setVariableValue( const std::string &VarName, const std::string &Value );
      void setVariableValue( const std::string &VarName, int Value );
      void setVariableValue( const std::string &VarName, double Value );

      std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const;
      double doubleVariable( const std::string &VarName, double DefaultValue = 0 ) const;
      int integerVariable( const std::string &VarName, int DefaultValue = 0 ) const;
  };

  // =====================================================
  

}


