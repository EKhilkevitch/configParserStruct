
#pragma once

// =====================================================

#include <string>
#include <vector>
#include <set>

// =====================================================

namespace configParserStruct
{
  
  // =====================================================
  
  class program;
  class variable;
  class reference;

  // =====================================================

  class parser 
  {
    public:
      static const char *const LastExpressionValueName;
      static const int SuccesssErrorLine;

      enum variableType
      {
        VarNone,
        VarUndef,
        VarInteger,
        VarReal,
        VarString,
        VarDict,
        VarArray,
        VarOther
      };

    private:
      program *Program;

    private:
      const variable* findVariable( const std::string &Name ) const;
      void setVariable( const std::string &Name, const variable &Value );
      static reference referenceForName( std::vector<char> *Name );

    public:
      parser();
      parser( const parser& );
      parser& operator=( const parser& );
      ~parser();
      void swap( parser &Other );

      void exec( const std::string &ConfigText );
      bool isVariableExist( const std::string &Name ) const;
      enum variableType variableType( const std::string &Name ) const;

      void build( const std::string &ConfigText );
      void optimize();
      void run();
      
      void reset();
      
      void setVariable( const std::string &Name, const std::string &Value );
      void setVariable( const std::string &Name, int Value );
      void setVariable( const std::string &Name, double Value );

      std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const;
      double doubleVariable( const std::string &VarName, double DefaultValue = 0 ) const;
      int integerVariable( const std::string &VarName, int DefaultValue = 0 ) const;

      std::set<std::string> variables() const;
      std::set<std::string> dictKeys( const std::string &Name ) const;

      int errorLine() const;

      std::string toDebugString() const;
  };

  // =====================================================

}


