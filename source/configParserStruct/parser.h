
#pragma once

// =====================================================

#include <string>
#include <set>
#include <list>

// =====================================================

/**
 * @file
 * @brief Базовый класс для интерпретаторов конфигурации.
 */

// =====================================================

namespace configParserStruct
{

  /**
   * @brief
   * Базовый класс для интерпретаторов конфигурации.
  */
  class parser
  {
    public:
      typedef std::set<std::string> containerForVariables;

    protected:
      virtual void setVariableValueString( const std::string &VarName, const std::string &Value );

    public:
      virtual ~parser() {}

      virtual void setVariableValue( const std::string &VarName, const std::string &Value );
      virtual void setVariableValue( const std::string &VarName, int Value );
      virtual void setVariableValue( const std::string &VarName, double Value );

      virtual void exec( const std::string &ConfigText ) = 0;
      virtual void execFile( const std::string &FileName );

      virtual bool isVariableExist( const std::string &VarName ) const = 0;
      
      virtual containerForVariables listOfVariables() const = 0;
      virtual containerForVariables listOfVariablesStruct() const;

      virtual std::string stringVariable( const std::string &VarName, const std::string &DefaultValue = std::string() ) const = 0;
      virtual double doubleVariable( const std::string &VarName, double DefaultValue = 0 ) const;
      virtual int integerVariable( const std::string &VarName, int DefaultValue = 0 ) const;
      template <class Enum> Enum enumVariable( const std::string &VarName ) const;
      template <class Enum> Enum enumVariable( const std::string &VarName, Enum DefaultValue ) const;
      
      static std::list<std::string> readFileContent( const std::string &FileName );
      static std::string joinStringList( const std::list<std::string> &List, const std::string &JoinString );

      static std::string structSeparator() { return "."; }
  };

  // =====================================================
  
  template <class Enum> Enum parser::enumVariable( const std::string &VarName ) const
  { 
    return static_cast<Enum>( integerVariable(VarName) ); 
  }

  template <class Enum> Enum parser::enumVariable( const std::string &VarName, Enum DefaultValue ) const 
  { 
    return static_cast<Enum>( integerVariable(VarName,DefaultValue) ); 
  }   

  // =====================================================
  

}


