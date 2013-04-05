
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

#endif

