
#pragma once

// =====================================================

#include "configParserStruct/parser.h"

// =====================================================

namespace configParserStruct
{

  namespace structParserUtil
  {
    class program;
  };

  class structParser : public parser
  {
    private:
      structParserUtil::program *Program;

    protected:
      void setVariableValueString( const std::string &VarName, const std::string &Value ) {}

    public:
      structParser( const std::string &ConfigText = std::string() );

      void exec( const std::string &ConfigText );
      bool isVariableExist( const std::string &VarName ) const;
      
      containerForVariables listOfVariables() const {}

      std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const;
      double doubleVariable( const std::string &VarName, double DefaultValue = 0 ) const;
      int integerVariable( const std::string &VarName, int DefaultValue = 0 ) const;
  };

  // =====================================================
  

}


