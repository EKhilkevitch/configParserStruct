
#pragma once

// =====================================================

#include "configParserStruct/parser.h"

// =====================================================

namespace configParserStruct
{

  namespace structParserUtil { class program; };

  class structParser : public parser
  {
    private:
      structParserUtil::program *Program;

    private:
      structParser( const structParser& );
      structParser& operator=( const structParser& );

    public:
      structParser( const std::string &ConfigText = std::string() );
      ~structParser();

      void exec( const std::string &ConfigText );
      bool isVariableExist( const std::string &VarName ) const;

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


