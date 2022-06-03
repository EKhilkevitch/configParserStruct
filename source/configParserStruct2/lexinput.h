
#ifndef _CPSSPU_LEXINPUT_H_
#define _CPSSPU_LEXINPUT_H_ 1

#include <stddef.h>

#if __cplusplus
extern "C" {
#endif

/* ===================================================== */

int CPSSPU_setParserInputString( const char *String );
void CPSSPU_clearParserInputString( void );
size_t CPSSPU_takeNextFromParserInputString( char *Buffer, size_t MaxSize );

/* ===================================================== */

#if __cplusplus
}
#endif


#endif

