
#ifndef _STRUCTPARSERCOMPILER_H_
#define _STRUCTPARSERCOMPILER_H_ 1

// =====================================================

#if __cplusplus
extern "C" {
#endif

#include <stdlib.h>

// CPSSPU = configParserStruct::structParserUtil 

void CPSSPU_popValueFromStack( void );
void CPSSPU_pushRealNumberToStack( double Number );
void CPSSPU_pushIntegerNumberToStack( int Number );
void CPSSPU_pushStringToStack( const char *String );
void CPSSPU_pushDictToStack( void );

void CPSSPU_beginOfNewFunctionAssignName( const char *Name );
void CPSSPU_endOfCurrentFunction( void );
void CPSSPU_returnFromCurrentFunction( void );

void CPSSPU_prepareToFunctionCall( void );
void CPSSPU_pushFunctionArgument( void );
void CPSSPU_callFunctionWithArgsFromStack( const char *Name );
void CPSSPU_pushVariableValueToStack( const char *Name );

void CPSSPU_beginOfIfStatement( void );
void CPSSPU_beginOfElseStatement( void );
void CPSSPU_endOfIfElseStatement( void );
void CPSSPU_endOfIfStatement( void );

void CPSSPU_assignVariableValueFromStack( const char *Name );
void CPSSPU_setDictFieldFromStack( const char *Name );
void CPSSPU_operatorOnStackTop( const char *OperatorType );
void CPSSPU_finalizeExpressionStack( void );

void CPSSPU_setParsingErrorLine( int LineNumber );
void CPSSPU_setStructParserError( void );

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

