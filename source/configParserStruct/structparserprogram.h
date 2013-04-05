
#pragma once

// =====================================================

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparsercommand.h"

// =====================================================

namespace configParserStruct
{
  namespace structParserUtil
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

        const variable getNamedVariable( const std::string &Name ) const { return Variables.get(Name); }
        void setNamedVariable( const std::string &Name, const variable &Value ) { Variables.set(Name,Value); }

        void clear();

        void build( const std::string &ProgramText );
        void execute() { Commands.execute( this ); }
    };
  
    // =====================================================
    
  }
}

// =====================================================

