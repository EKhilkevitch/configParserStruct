
#pragma once

// =====================================================

#include <string>
#include <map>
#include <stack>

#include "configParserStruct/stringcast.h"
#include "configParserStruct/clonablepointer.h"

// =====================================================

namespace configParserStruct
{
  namespace structParser
  {
    class undefVariableValue;
    
    // =====================================================
   
    class variableValue
    {
      public:
        virtual ~variableValue() {}
        virtual variableValue* clone() const = 0;
        virtual const std::string string() const = 0;
        virtual double number() const = 0;
        virtual bool boolean() const = 0;
    };
    
    // =====================================================

    class variable
    {
      private:
        clonablePointer<variableValue,undefVariableValue> Value;

      public:
        variable() {}
        variable( const variableValue &V ) : Value(V) {}

        const std::string string() const { return Value->string(); }
        double number() const { return Value->number(); }
        bool boolean() const { return Value->boolean(); }
    };

    template <class T> variable createVariable( const T &Arg );
    template <> variable createVariable( const double &Arg );
    
    // =====================================================
    
    class variablesList
    {
      private:
        std::map< std::string, variable > Variables;

      public:
        variablesList() {}

        void set( const std::string &Name, const variable &Var );
        const variable get( const std::string &Name ) const;
    };
    
    // =====================================================
    
    class variablesStack
    {
      private:
        std::stack< variable > Stack;

      public:
        variablesStack() {}

        void push( const variable &Var );
        const variable pop();
        const variable top() const;
    };
    
    // =====================================================

    class undefVariableValue : public variableValue
    {
      public:
        undefVariableValue() {}
        
        variableValue* clone() const { return new undefVariableValue(); }
        const std::string string() const { return std::string(); }
        double number() const { return 0; }
        bool boolean() const { return false; }
    };
    
    // -----------------------------------------------------

    class realVariableValue : public variableValue
    {
      private:
        double Value;
      public:
        realVariableValue( double V ) : Value(V) {}
        variableValue* clone() const { return new realVariableValue(Value); }
        const std::string string() const { return convertToString(Value); }
        double number() const { return Value; }
        bool boolean() const { return Value != 0.0; }
    };
    
    // =====================================================
    
    template <> inline variable createVariable( const double &Arg )
    {
      realVariableValue Value( Arg );
      return variable( Value );
    }
    
    // =====================================================

  }
}

// =====================================================

