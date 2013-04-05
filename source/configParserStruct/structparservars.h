
#pragma once

// =====================================================

#include <string>
#include <map>
#include <vector>
#include <list>

#include "configParserStruct/stringcast.h"
#include "configParserStruct/clonablepointer.h"

// =====================================================

namespace configParserStruct
{
  namespace structParserUtil
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
        bool isDefined() const;
    };

    template <class T> variable createVariable( const T &Arg );
    template <> variable createVariable( const double &Arg );
    template <> variable createVariable( const int &Arg );
    template <> variable createVariable( const char* const &Arg );
    
    // =====================================================

    class undefVariableValue : public variableValue
    {
      public:
        undefVariableValue() {}
        
        variableValue* clone() const { return new undefVariableValue(*this); }
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
        variableValue* clone() const { return new realVariableValue(*this); }
        const std::string string() const { return convertToString(Value); }
        double number() const { return Value; }
        bool boolean() const { return Value != 0.0; }
    };
    
    // -----------------------------------------------------
    
    class stringVariableValue : public variableValue
    {
      private:
        std::string Value;
      public:
        stringVariableValue( const std::string &V ) : Value(V) {}
        variableValue* clone() const { return new stringVariableValue(*this); }
        const std::string string() const { return Value; }
        double number() const { return convertFromString<double>(Value); }
        bool boolean() const { return Value.empty(); }
    };
    
    // -----------------------------------------------------
    
    class dictVariableValue : public variableValue
    {
      private:
        std::map< std::string, variable > Dict;

      public:
        dictVariableValue() {}
        variableValue* clone() const { return new dictVariableValue(*this); }

        const std::string string() const;
        double number() const { return Dict.size(); }
        bool boolean() const { return ! Dict.empty(); }

        void addItem( const std::string &Key, const variable &Value );
        const variable getItem( const std::string &Key ) const;
        variable* getItemPointer( const std::string &Key );
        size_t numberOfItems() const { return Dict.size(); }
        void clear() { Dict.clear(); }
        const std::list<std::string> listOfKeys() const;
    };
        
    // =====================================================
    
    class variablesList
    {
      private:
        dictVariableValue Dict;

      public:
        variablesList() {}

        void set( const std::string &Name, const variable &Var ) { Dict.addItem( Name, Var ); }
        const variable get( const std::string &Name ) const { return Dict.getItem( Name ); }

        std::list<std::string> listOfNames() const { return Dict.listOfKeys(); }
        size_t size() const { return Dict.numberOfItems(); }

        void clear() { Dict.clear(); }
    };
    
    // =====================================================
    
    class variablesStack
    {
      private:
        std::vector< variable > Stack;

      public:
        variablesStack() {}

        void push( const variable &Var );
        const variable pop();
        const variable top() const;

        size_t size() const { return Stack.size(); }
        void clear() { Stack.clear(); }
    };

    // =====================================================
    
    template <> inline variable createVariable( const double &Arg )
    {
      realVariableValue Value( Arg );
      return variable( Value );
    }
    
    // -----------------------------------------------------
    
    template <> inline variable createVariable( const int &Arg )
    {
      return createVariable<double>( Arg );
    }
    
    // -----------------------------------------------------
    
    template <> inline variable createVariable( const char* const &Arg )
    {
      std::string String = ( Arg == NULL ) ? "" : Arg;
      stringVariableValue Value( Arg );
      return variable( Value );
    }

    // =====================================================

  }
}

// =====================================================

