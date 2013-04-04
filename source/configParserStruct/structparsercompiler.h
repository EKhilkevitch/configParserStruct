
#ifndef _STRUCTPARSERCOMPILER_H_
#define _STRUCTPARSERCOMPILER_H_ 1

// =====================================================

#if __cplusplus
extern "C" {
#endif

#include <stdlib.h>

void popValueFromStack();
void pushRealNumberToStack( double Number );
void pushIntegerNumberToStack( int Number );
void addValuesFromStack();

#if __cplusplus
}
#endif

// =====================================================

#if __cplusplus

#include <vector>
#include <map>
#include <string>

#include "configParserStruct/stringcast.h"

namespace configParserStruct
{
  namespace structParser
  {
    class program;
    class undefVariableValue;

    // =====================================================
   
    class commandAction
    {
      public:
        virtual ~commandAction() {}
        virtual commandAction* clone() const = 0;
        virtual void execute( program *Program ) const = 0; 
    };
    
    // =====================================================
    
    class command 
    {
      private:
        commandAction *Action;

      public:
        command();
        command( const commandAction &A );
        command( const command &C );
        command& operator=( const command &C );
        ~command();

        void execute( program *Program ) const { Action->execute(Program); }
    };
    
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
        variableValue *Value;
        std::string Name;

      public:
        variable();
        variable( const variableValue &V, const std::string &Name = std::string() );
        variable( const variable& V );
        variable& operator=( const variable& V );
        ~variable();

        const std::string& name() const { return Name; }

        const std::string string() const { return Value->string(); }
        double number() const { return Value->number(); }
        bool boolean() const { return Value->boolean(); }
    };
    
    // =====================================================

    class program
    {
      private:
        std::vector< command > Commands;
        std::map< std::string, variable > Variables;
        std::vector< variable > Stack;

      private:
        program( const program& );
        program& operator=( const program& );

      public:
        program();
        ~program();

        unsigned pushCommand( const commandAction &A ) { Commands.push_back(command(C)); }

        void pushVariable( const variableValue &V ) { pushVariable(variable(V)); }
        void pushVariable( const variable &V ) { Stack.push_back(V); }
        variable popVariable();
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
    
    class nopCommand : public commandAction
    {
      public:
        void execute( program* ) const {}
        commandAction* clone() const { return new nopCommand(); }
    };

    // -----------------------------------------------------
    
    class pushCommand : public commandAction
    {
      private:
        variable Variable;

      public:
        pushCommand( const variable &V ) : Variable(V) {}
        void execute( program *Program ) const { Program->pushVariable( Variable ); }
        commandAction* clone() const { return new pushCommand(Variable); }
    };
    
    // -----------------------------------------------------
    
    class popCommand : public commandAction
    {
      public:
        void execute( program *Program ) const { Program->popVariable(); }
        commandAction* clone() const { return new popCommand(); }
    };
    
    // -----------------------------------------------------

    class addCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new addCommand(); }
    };

    // =====================================================

  }
}

#endif

// =====================================================

#endif

