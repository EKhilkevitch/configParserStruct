
#ifndef _STRUCTPARSERCOMPILER_H_
#define _STRUCTPARSERCOMPILER_H_ 1

// =====================================================

#if __cplusplus
extern "C" {
#endif

#include <stdlib.h>

void popValueFromStack( void );
void pushRealNumberToStack( double Number );
void pushIntegerNumberToStack( int Number );
void pushStringToStack( const char *String );
void pushDictToStack( void );
void beginOfNewFunctionAssignName( const char *Name );
void endOfCurrentFunction( void );
void returnFromCurrentFunction( void );
void prepareToFunctionCall( void );
void pushFunctionArgument( void );
void callFunctionWithArgsFromStack( const char *Name );
void pushVariableValueToStack( const char *Name );
void assignVariableValueFromStack( const char *Name );
void setDictFieldFromStack( const char *Name );
void operatorOnStackTop( int OperatorType );
void finalizeExpressionStack( void );
void setParsingErrorLine( int LineNumber );
void setStructParserError( void );

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

