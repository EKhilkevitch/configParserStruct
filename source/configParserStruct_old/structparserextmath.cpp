
// =====================================================

#include "configParserStruct/structparserextmath.h"
#include <cmath>

#if __cplusplus <= 199711L || ( _WIN32 && _MSC_VER >= 1700 )
  #define USE_STD_IMPL 0
#else
  #define USE_STD_IMPL 1
#endif

// =====================================================

double configParserStruct::structParserUtil::erf( double x )
{
#if USE_STD_IMPL
  return std::erf( x );
#else
  // http://stackoverflow.com/questions/6281020/error-function-erfx-not-found-in-math-h-for-visual-studio-2005
  // http://stackoverflow.com/questions/457408/is-there-an-easily-available-implementation-of-erf-for-python
  double t = 1.0 / (1.0 + 0.5 * std::fabs(x));
  double y = 1.0 - t * std::exp( -x*x - 1.26551223 +
                                  t * ( 1.00002368 +
                                  t * ( 0.37409196 + 
                                  t * ( 0.09678418 + 
                                  t * (-0.18628806 + 
                                  t * ( 0.27886807 + 
                                  t * (-1.13520398 + 
                                  t * ( 1.48851587 + 
                                  t * (-0.82215223 + 
                                  t * ( 0.17087277))))))))));
  return ( x >= 0 ) ? y : -y;
#endif
}

// -----------------------------------------------------

double configParserStruct::structParserUtil::erfc( double x )
{
#if USE_STD_IMPL
  return std::erfc( x );
#else
  // http://stackoverflow.com/questions/457408/is-there-an-easily-available-implementation-of-erf-for-python
  double t = 1.0 / (1.0 + 0.5 * std::fabs(x));
  double z =       t * std::exp( -x*x - 1.26551223 +
                                  t * ( 1.00002368 +
                                  t * ( 0.37409196 + 
                                  t * ( 0.09678418 + 
                                  t * (-0.18628806 + 
                                  t * ( 0.27886807 + 
                                  t * (-1.13520398 + 
                                  t * ( 1.48851587 + 
                                  t * (-0.82215223 + 
                                  t * ( 0.17087277))))))))));
  return ( x >= 0 ) ? z : 2 - z;
#endif
}

// =====================================================

