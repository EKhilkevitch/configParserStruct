
/* ===================================================== */

#include "configParserStruct/snprintf.h"

#include <stdarg.h>
#include <stdio.h>

/*
 * https://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
 */

#if _WIN32
/* ===================================================== */

int c99_vsnprintf( char *outBuf, size_t size, const char *format, va_list ap )
{
  int count = -1;

  if (size != 0)
    count = _vsnprintf_s(outBuf, size, _TRUNCATE, format, ap);
  
  if (count == -1)
    count = _vscprintf(format, ap);

  return count;
}

/* ===================================================== */

int c99_snprintf( char *outBuf, size_t size, const char *format, ... )
{
  int count;
  va_list ap;

  va_start(ap, format);
  count = c99_vsnprintf(outBuf, size, format, ap);
  va_end(ap);

  return count;
}

/* ===================================================== */
#endif

