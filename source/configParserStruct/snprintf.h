
#ifndef _CPSSPU_SNPRINTF_
#define _CPSSPU_SNPRINTF_

#include <stdio.h>

#if __cplusplus
#  define SN_EXTERN extern "C"
#else
#  define SN_EXTERN
#endif

/* ===================================================== */

#if defined(_MSC_VER) && _MSC_VER < 1900
#  define snprintf c99_snprintf
#  define vsnprintf c99_vsnprintf
#endif

#if _WIN32
SN_EXTERN int c99_snprintf(char *Buffer, size_t Size, const char *Format, ... );
SN_EXTERN int c99_vsnprintf(char *Buffer, size_t Size, const char *Format, va_list Args );
#endif

/* ===================================================== */


#undef SN_EXTERN

#endif

