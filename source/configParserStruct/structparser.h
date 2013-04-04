
#pragma once

// =====================================================

#include "configParserStruct/parser.h"

// =====================================================

namespace configParserStruct
{

  class structParser : public parser
  {
    protected:
      void setVariableValueString( const std::string &VarName, const std::string &Value ) {}

    public:
      structParser( const std::string &ConfigText = std::string() );

      void exec( const std::string &ConfigText ) {}
      bool isVariableExist( const std::string &VarName ) const {}
      
      containerForVariables listOfVariables() const {}

      std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const {}
  };

  // =====================================================
  

}


