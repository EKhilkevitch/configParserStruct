
#pragma once

// =====================================================

#include "configParserStruct/reference.h"

#include <string>
#include <vector>
#include <map>

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class variable;
  class reference;

  // =====================================================

  class variableValue
  {
    public:
      virtual ~variableValue() = 0;

      virtual variableValue* clone( void *Memory = NULL ) const = 0;
      virtual const char* type() const = 0;

      virtual int integer() const = 0;
      virtual double real() const = 0;
      virtual std::string string() const = 0;
      virtual bool boolean() const = 0;

      virtual reference ref() const;
      virtual variable* getByRef( const reference &Reference ) const;
      virtual variable* setByRef( const reference &Reference, const variable &Variable );
  };

  // =====================================================

  class undefVariableValue : public variableValue
  {
    public:
      undefVariableValue();
      
      undefVariableValue* clone( void *Memory = NULL ) const;
      const char* type() const;

      int integer() const;
      double real() const;
      std::string string() const;
      bool boolean() const;
      
      static const char *const TypeName;
  };

  // =====================================================
  
  class integerVariableValue : public variableValue
  {
    private:
      int Value;

    public:
      explicit integerVariableValue( int Value );
     
      integerVariableValue* clone( void *Memory = NULL ) const;
      const char* type() const;

      int integer() const;
      double real() const;
      std::string string() const;
      bool boolean() const;

      static const char *const TypeName;
  };

  // =====================================================
  
  class realVariableValue : public variableValue
  {
    private:
      double Value;

    public:
      explicit realVariableValue( double Value );
     
      realVariableValue* clone( void *Memory = NULL ) const;
      const char* type() const;

      int integer() const;
      double real() const;
      std::string string() const;
      bool boolean() const;
      
      static const char *const TypeName;
  };


  // =====================================================
  
  class stringVariableValue : public variableValue
  {
    private:
      char *Value;

    private:
      stringVariableValue( const stringVariableValue& );
      stringVariableValue& operator=( const stringVariableValue& );

    public:
      explicit stringVariableValue( const char *Value );
      ~stringVariableValue();
     
      stringVariableValue* clone( void *Memory = NULL ) const;
      const char* type() const;

      int integer() const;
      double real() const;
      std::string string() const;
      bool boolean() const;
      
      static const char *const TypeName;
  };
    
  // =====================================================
  
  class referenceVariableValue : public variableValue
  {
    private:
      reference Reference;

    public:
      explicit referenceVariableValue( const reference &Reference );
      explicit referenceVariableValue( const char *Value, reference::type Type );
      explicit referenceVariableValue( size_t Value, reference::type Type );

      referenceVariableValue* clone( void *Memory = NULL ) const;
      const char* type() const;

      int integer() const;
      double real() const;
      std::string string() const;
      bool boolean() const;

      reference ref() const;
      
      static const char *const TypeName;
  };

  // =====================================================
  
  class arrayVariableValue : public variableValue
  {
    private:
      mutable std::vector< variable > Array;

    private:
      arrayVariableValue( const arrayVariableValue& );
      arrayVariableValue& operator=( const arrayVariableValue& );

    private:
      explicit arrayVariableValue( const std::vector< variable > &Array );

    public:
      explicit arrayVariableValue( size_t Size );
      ~arrayVariableValue();

      arrayVariableValue* clone( void *Memory = NULL ) const;
      const char* type() const;

      int integer() const;
      double real() const;
      std::string string() const;
      bool boolean() const;
      
      variable* getByRef( const reference &Reference ) const;
      variable* setByRef( const reference &Reference, const variable &Variable );
      
      static const char *const TypeName;
  };

  // =====================================================
  
  class dictVariableValue : public variableValue
  {
    public:
      static const char *const ArrayOfKeysName;

    private:
      mutable std::map< std::string, variable > *Dict;
      mutable variable *KeysArray;

    private:
      dictVariableValue( const dictVariableValue& );
      dictVariableValue& operator=( const dictVariableValue& );

    private:
      void updateKeysArray() const;

    private:
      explicit dictVariableValue( const std::map< std::string, variable > &Dict );

    public:
      dictVariableValue();
      ~dictVariableValue();

      dictVariableValue* clone( void *Memory = NULL ) const;
      const char* type() const;

      int integer() const;
      double real() const;
      std::string string() const;
      bool boolean() const;
      
      variable* getByRef( const reference &Reference ) const;
      variable* setByRef( const reference &Reference, const variable &Variable );
      
      static const char *const TypeName;
  };

  // =====================================================

}

