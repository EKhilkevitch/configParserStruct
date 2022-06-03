
#pragma once

// =====================================================

#include <string>

// =====================================================

namespace configParserStruct
{
  
  // =====================================================
  
  class program;

  // =====================================================

  class parser 
  {
    public:
      static const char *const LastExpressionValueName;

    private:
      program *Program;

    private:
      parser( const parser& );
      parser& operator=( const parser& );

    public:
      parser();
      ~parser();

      void exec( const std::string &ConfigText );
      bool isVariableExist( const std::string &VarName ) const;

      void build( const std::string &ConfigText );
      void run();
      
      void reset();
      
      void setVariable( const std::string &Name, const std::string &Value );
      void setVariable( const std::string &Name, int Value );
      void setVariable( const std::string &Name, double Value );

      std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const;
      double doubleVariable( const std::string &VarName, double DefaultValue = 0 ) const;
      int integerVariable( const std::string &VarName, int DefaultValue = 0 ) const;
  };

  // =====================================================

}


