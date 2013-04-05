
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
void pushStringToStack( const char *String );
void pushVariableValueToStack( const char *Name );
void assignVariableValueFromStack( const char *Name );
void operatorOnStackTop( int OperatorType );

#if __cplusplus
}
#endif

// =====================================================

#if __cplusplus
namespace configParserStruct
{
  namespace structParserUtil
  {
    class program;

    void setStructPrserProgram( program *const Program );
  }
}

#endif

// =====================================================

#endif

