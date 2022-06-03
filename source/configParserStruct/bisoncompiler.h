
#ifndef _CPSSPU_COMPILER_H_
#define _CPSSPU_COMPILER_H_ 1

#if __cplusplus
extern "C" {
#endif

/* ===================================================== */

/* 'CPSSPU' means configParserStruct::structParserUtil */

void CPSSPU_pushRealNumberToStack( double Number );
void CPSSPU_pushIntegerNumberToStack( int Number );
void CPSSPU_pushStringToStack( const char *String );
void CPSSPU_pushDictToStack( void );
void CPSSPU_pushArrayToStack( void );

void CPSSPU_beginOfNewFunctionAssignName( void );
void CPSSPU_endOfCurrentFunction( void );
void CPSSPU_returnFromCurrentFunction( void );
void CPSSPU_endOfProgram( void );

void CPSSPU_prepareToFunctionCall( void );
void CPSSPU_pushFunctionArgument( void );
void CPSSPU_callFunctionWithArgsFromStack( const char *Name );
void CPSSPU_pushVariableValueToStack( void );
void CPSSPU_pushVariableReferenceToStack( const char *Name );
void CPSSPU_pushVariableGlobalReferenceToStack( const char *Name );
void CPSSPU_pushArgumentReferenceToStack( int Number );
void CPSSPU_replaceReferenceToValueOnStack( void );
void CPSSPU_setDictKeyString( const char *Name );
void CPSSPU_setArrayIndexFromTop( void );
void CPSSPU_setDictKeyFromTop( void );
void CPSSPU_pushVariableSymbolicReferenceToStack( void );

void CPSSPU_beginOfIfStatement( void );
void CPSSPU_beginOfElseStatement( void );
void CPSSPU_endOfIfElseStatement( void );
void CPSSPU_endOfIfStatement( void );
void CPSSPU_prepareForWhileStatement( void );
void CPSSPU_beginOfWhileStatement( void );
void CPSSPU_endOfWhileStatement( void );

void CPSSPU_assignVariableValueFromStack( void );
void CPSSPU_setDictFieldFromStack( const char *Name );
void CPSSPU_setArrayElementFromStack( void );
void CPSSPU_operatorOnStackTop( const char *OperatorType );
void CPSSPU_finalizeExpressionStack( void );

void CPSSPU_setStructParserError( void );

/* ===================================================== */

#if __cplusplus
}
#endif

/* ===================================================== */

#if __cplusplus
namespace configParserStruct
{
  class text;
}
  
void CPSSPU_setTextForBison( configParserStruct::text *Text );
#endif

/* ===================================================== */

#endif

