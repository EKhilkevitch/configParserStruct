
#pragma once

// =====================================================

#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <typeinfo>
#include <deque>

#include "configParserStruct/stringcast.h"
#include "configParserStruct/clonablepointer.h"

// =====================================================

namespace configParserStruct
{
  namespace structParserUtil
  {
    
    class program;
    class variable;
    class undefVariableValue;
    class referenceVariableValue;
    
    // =====================================================
   
    class variableValue
    {
      public:
        virtual ~variableValue() {}
        virtual variableValue* clone() const = 0;
        virtual const std::string string() const = 0;
        virtual double number() const = 0;
        virtual int integer() const = 0;
        virtual bool boolean() const = 0;
        virtual const variable valueByReference( const variable &Reference, unsigned AttrLevel = 0 ) const;
    };
    
    // =====================================================
    
    template <class T> T extractValueScalar( const variable &Var );

    class variable
    {
      private:
        clonablePointer<variableValue,undefVariableValue> Value;

      public:
        variable();
        variable( const variableValue &Value );

        const std::string string() const { return Value->string(); }
        double number() const { return Value->number(); }
        int integer() const { return Value->integer(); }
        bool boolean() const { return Value->boolean(); }
        bool isDefined() const;
       
        const variable valueByReference( const variable &Reference, unsigned AttrLevel = 0 ) const;
        void setValueByReference( const variable &Reference, const variable &Value, unsigned AttrLevel = 0 );
        
        const std::type_info& valueType() const { return typeid(*Value); }
        template <class VarT> bool isValueDerivedFrom() const { return Value.isDerivedFrom<VarT>(); }
        
        template <class VarT > VarT& value() { return dynamic_cast<VarT&>( *Value ); }
        template <class VarT > const VarT& value() const { return dynamic_cast<const VarT&>( *Value ); }

        template <class T> T valueScalar() const { return extractValueScalar<T>(*this); }   
    };

    template <> inline double extractValueScalar( const variable &Var ) { return Var.number(); }
    template <> inline int extractValueScalar( const variable &Var ) { return Var.integer(); }
    template <> inline bool extractValueScalar( const variable &Var ) { return Var.boolean(); }
    template <> inline std::string extractValueScalar( const variable &Var ) { return Var.string(); }

    template <class T> variable createVariable( const T &Arg );
    template <> variable createVariable( const double &Arg );
    template <> variable createVariable( const int &Arg );
    template <> variable createVariable( const bool &Arg );
    template <> variable createVariable( const char* const &Arg );
    template <> variable createVariable( const std::string &Arg );

    // =====================================================

    class undefVariableValue : public variableValue
    {
      public:
        undefVariableValue() {}
        
        variableValue* clone() const { return new undefVariableValue(*this); }

        const std::string string() const { return std::string(); }
        double number() const { return 0; }
        int integer() const { return 0; }
        bool boolean() const { return false; }
    };
    
    // -----------------------------------------------------
    
    class realVariableValue : public variableValue
    {
      private:
        double Value;

      public:
        explicit realVariableValue( double V ) : Value(V) {}
        
        variableValue* clone() const { return new realVariableValue(*this); }
        
        const std::string string() const { return convertToString(Value); }
        double number() const { return Value; }
        int integer() const { return static_cast<int>(Value); }
        bool boolean() const { return Value != 0.0; }
    };
    
    // -----------------------------------------------------
    
    class integerVariableValue : public variableValue
    {
      private:
        int Value;

      public:
        explicit integerVariableValue( int V ) : Value(V) {}
        
        variableValue* clone() const { return new integerVariableValue(*this); }
        
        const std::string string() const { return convertToString(Value); }
        double number() const { return Value; }
        int integer() const { return Value; }
        bool boolean() const { return Value != 0; }
    };
    
    // -----------------------------------------------------
    
    class stringVariableValue : public variableValue
    {
      private:
        std::string Value;

      public:
        explicit stringVariableValue( const std::string &V ) : Value(V) {}
        
        variableValue* clone() const { return new stringVariableValue(*this); }
        
        const std::string string() const { return Value; }
        double number() const { return convertFromString<double>(Value); }
        int integer() const { return convertFromString<int>(Value); }
        bool boolean() const { return ! Value.empty(); }
    };
    
    // -----------------------------------------------------
    
    class commandAddressVariableValue : public variableValue
    {
      private:
        size_t CommandIndex;

      public:
        explicit commandAddressVariableValue( size_t Index ) : CommandIndex(Index) {}
        
        variableValue* clone() const { return new commandAddressVariableValue(*this); }
        
        const std::string string() const { return convertToString(CommandIndex); }
        double number() const { return CommandIndex; }
        int integer() const { return CommandIndex; }
        bool boolean() const { return true; }
        size_t address() const { return CommandIndex; }
    };
    
    // -----------------------------------------------------
    
    class referenceVariableValue : public variableValue
    {
      private:
        std::string Name;
        std::vector< variable > Attributes;

      public:
        explicit referenceVariableValue( const std::string &N ) : Name(N) {}
        
        variableValue* clone() const { return new referenceVariableValue(*this); }
        
        const std::string string() const;
        double number() const { return 0; }
        int integer() const { return 0; }
        bool boolean() const { return true; }

        const std::string& name() const { return Name; }
        void pushAttribute( const variable &Var ) { Attributes.push_back(Var); }
        void popAttribute() { if ( ! Attributes.empty() ) Attributes.pop_back(); }
        const variable& attribute( unsigned Index ) const { return Attributes.at(Index); }
        size_t numberOfAttributes() const { return Attributes.size(); }

        const variable getValue( const program &Program ) const;
        void setValue( program *Program, const variable &Value ) const;
    };
    
    // -----------------------------------------------------
    
    class builtinFunctionValue : public variableValue
    {
      protected:
        size_t getNumberOfArguments( const program &Program ) const;
        const variable getArgument( size_t Index, const program &Program ) const;
        const variable getVariable( const std::string &Name, const program &Program ) const;

      public:
        virtual const variable execute( const program &Program ) const = 0;
        
        variableValue* clone() const = 0;

        const std::string string() const = 0;

        double number() const { return 0; }
        int integer() const { return 0; }
        bool boolean() const { return true; }
    };
    
    // -----------------------------------------------------
   
    class composerVariableValue : public variableValue
    {
      public:
        virtual variable* getItemPointerByVariableKey( const variable &Key ) = 0;
        virtual const variable getItemByVariableKey( const variable &Key ) const = 0;
        virtual void addItemByVariableKey( const variable &Key, const variable &Value ) = 0;

      public:
        const variable valueByReference( const variable &Reference, unsigned AttrLevel = 0 ) const;
        void setValueByReference( const variable &Reference, const variable &Value, unsigned AttrLevel = 0 );

        virtual size_t numberOfItems() const = 0;
        virtual void clear() = 0;
        
        static variable* getItemPointer( variable *Variable, const std::string &KeySuffix );
        static std::pair<std::string,std::string> splitKey( const std::string &Key );
    };

    // -----------------------------------------------------
    
    class arrayVariableValue : public composerVariableValue
    {
      private:
        std::deque< variable > Array;

      public:
        arrayVariableValue() {}

        variableValue* clone() const { return new arrayVariableValue(*this); }
        
        const std::string string() const;
        double number() const { return Array.size(); }
        int integer() const { return Array.size(); }
        bool boolean() const { return ! Array.empty(); }

        void pushItem( const variable &Value );
        void addItem( size_t Index, const variable &Value );
        const variable getItem( int Index ) const;
        variable* getItemPointer( int Index );
        size_t numberOfItems() const { return Array.size(); }
        void clear() { Array.clear(); }
        
        variable* getItemPointerByVariableKey( const variable &Key );
        const variable getItemByVariableKey( const variable &Key ) const;
        void addItemByVariableKey( const variable &Key, const variable &Value );

        static std::string arraySeparatorBegin();
        static std::string arraySeparatorEnd();
    };

    // -----------------------------------------------------
    
    class dictVariableValue : public composerVariableValue
    {
      private:
        std::map< std::string, variable > Dict;

      public:
        dictVariableValue() {}
        
        variableValue* clone() const { return new dictVariableValue(*this); }

        const std::string string() const;
        double number() const { return Dict.size(); }
        int integer() const { return Dict.size(); }
        bool boolean() const { return ! Dict.empty(); }

        void addItem( const std::string &Key, const variable &Value );
        const variable getItem( const std::string &Key ) const;
        variable* getItemPointer( const std::string &Key );
        size_t numberOfItems() const { return Dict.size(); }
        void clear() { Dict.clear(); }
        const std::set<std::string> listOfKeys() const;
        const std::set<std::string> listOfKeysIncludeSubdict() const;
        
        variable* getItemPointerNotFollow( const std::string &Key );
        variable* getItemPointerByVariableKey( const variable &Key );
        const variable getItemByVariableKey( const variable &Key ) const;
        void addItemByVariableKey( const variable &Key, const variable &Value );
        
        static std::string dictSeparator();
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
        variable* getPointer( const std::string &Name ) { return Dict.getItemPointer( Name ); }

        std::set<std::string> listOfNames() const { return Dict.listOfKeys(); }
        std::set<std::string> listOfNamesIncludeSubdict() const { return Dict.listOfKeysIncludeSubdict(); }
        size_t size() const { return Dict.numberOfItems(); }

        void clear() { Dict.clear(); }
    };
    
    // =====================================================
    
    class variablesListStack 
    {
      private:
        typedef std::list< variablesList > varListStack;

        varListStack Stack;

      public:
        variablesListStack();

        void set( const std::string &Name, const variable &Var );
        const variable get( const std::string &Name ) const;
        variable* getPointer( const std::string &Name );
        const variable getFromTopOfStack( const std::string &Name ) const;

        void pushNewList();
        void popList();
        
        std::set<std::string> listOfNames() const;
        std::set<std::string> listOfNamesInAllStack() const;
        size_t size() const { return Stack.size(); }

        void clear();

        static std::string globalPrefix();
        static std::string globalName( const std::string &Name ); 
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
        variable* topPointer();

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
      integerVariableValue Value( Arg );
      return variable( Value );
    }
    
    // -----------------------------------------------------
    
    template <> inline variable createVariable( const bool &Arg )
    {
      integerVariableValue Value( Arg ? 1 : 0 );
      return variable( Value );
    }
    
    // -----------------------------------------------------
    
    template <> inline variable createVariable( const char* const &Arg )
    {
      std::string String = ( Arg == NULL ) ? "" : Arg;
      return createVariable( String );
    }    

    // -----------------------------------------------------
    
    template <> inline variable createVariable( const std::string &Arg )
    {
      stringVariableValue Value( Arg );
      return variable( Value );
    }
    
    // =====================================================
  
  }
}

// =====================================================

