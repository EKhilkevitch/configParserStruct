
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
#include "configParserStruct/clonablepointer.h"
#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparsercommand.h"

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
    };
  
    // =====================================================

  }
}

#endif

// =====================================================

#endif

