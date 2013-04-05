
#pragma once

// =====================================================

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparsercommand.h"

// =====================================================

namespace configParserStruct
{
  namespace structParser
  {
    
    // =====================================================

    class program
    {
      private:
        commandsList Commands;
        variablesList Variables;
        variablesStack Stack;

      public:
        program();
        ~program();

        unsigned pushCommand( const commandAction &A ) { return Commands.push(A); }

        void pushStackVariable( const variableValue &V ) { Stack.push(V); }
        void pushStackVariable( const variable &V )      { Stack.push(V); }
        const variable popStackVariable() { return Stack.pop(); }
        const variable topStackVariable() const { return Stack.top(); }
        size_t stackSize() const { return Stack.size(); }

        const variable getNamedVariable( const std::string &Name ) { return Variables.get(Name); }
        void setNamedVariable( const std::string &Name, const variable &Value ) { Variables.set(Name,Value); }
    };
  
    // =====================================================
    
    class interpreter
    {
      private:
        program Program;

      public:
        interpreter() {}
        
        void build( const std::string &String );
        void execute();
    };
    
    // =====================================================

  }
}

// =====================================================

