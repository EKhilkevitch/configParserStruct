
#pragma once

// =====================================================

#include "configParserStruct/parser.h"

#include <map>
#include <utility>

// =====================================================

namespace configParserStruct
{

  class simpleParser : public parser
  {
    private:
      std::map< std::string, std::string > Variables;

    private:
      void execOneLine( std::string &String );
    
    protected:
      void setVariableValueString( const std::string &VarName, const std::string &Value );

    public:
      simpleParser( const std::string &ConfigText = std::string() );

      void exec( const std::string &ConfigText );
      bool isVariableExist( const std::string &VarName ) const;
      
      containerForVariables listOfVariables() const;

      std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const;
      
      static std::string stripSpaces( const std::string &String );
      static std::pair<std::string,std::string> splitLineOnNameandValue( const std::string &String );

  };

  // =====================================================
  

}


