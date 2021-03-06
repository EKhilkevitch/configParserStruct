
#pragma once

// =====================================================

#include "configParserStruct/exception.h"

#include <string>
#include <set>
#include <list>

// =====================================================

/**
 * @file
 * @brief ������� ����� ��� ��������������� ������������.
 */

// =====================================================

namespace configParserStruct
{

  /**
   * @brief
   * ������� ����� ��� ��������������� ������������.
  */
  class parser
  {
    public:
      struct exception : public ::configParserStruct::exception
      {
        exception( const std::string &What );
      };

      class openFileException : public exception
      {
        private:
          std::string FileName;
        public:
          openFileException( const std::string &FName, const std::string &Reason ); 
          ~openFileException() throw() {}
          const std::string &fileName() const { return FileName; }
      };

      class readFileException : public exception
      {
        private:
          std::string FileName;
        public:
          readFileException( const std::string &FName, const std::string &Reason ); 
          ~readFileException() throw() {}
          const std::string &fileName() const { return FileName; }
      };

    public:
      typedef std::set<std::string> containerForVariables;

    protected:
      virtual void setVariableValueString( const std::string &VarName, const std::string &Value );

    public:
      virtual ~parser();

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
      static std::string readFileContentinSingleString( const std::string &FileName, const std::string &JoinString );
      static std::list<std::string> splitString( const std::string &String, const std::string &PossibleSeparators );

      static std::string structSeparator();
  };

  // =====================================================
  
  template <class Enum> Enum parser::enumVariable( const std::string &VarName ) const
  { 
    return static_cast<Enum>( integerVariable(VarName) ); 
  }
  
  // -----------------------------------------------------

  template <class Enum> Enum parser::enumVariable( const std::string &VarName, Enum DefaultValue ) const 
  { 
    return static_cast<Enum>( integerVariable(VarName, static_cast<int>(DefaultValue) ) ); 
  }   

  // =====================================================
  

}


