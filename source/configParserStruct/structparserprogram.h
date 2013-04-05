
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

        void pushVariable( const variableValue &V ) { Stack.push(V); }
        void pushVariable( const variable &V )      { Stack.push(V); }
        const variable popVariable() { return Stack.pop(); }
        const variable topVariable() const { return Stack.top(); }
        size_t stackSize() const { return Stack.size(); }
    };
  
    // =====================================================

  }
}

// =====================================================

