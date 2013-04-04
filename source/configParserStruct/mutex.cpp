
// =====================================================

#include "configParserStruct/mutex.h"

#include <cassert>
#include <cstdlib>

#if __unix__
#  include <pthread.h>
#elif _WIN32
#  include <windows.h>
#else
#  error "Not implemented."
#endif

// =====================================================

#if __unix__
static inline pthread_mutex_t* castToMutex( void *Pointer ) { return static_cast<pthread_mutex_t*>( Pointer ); }
#endif

#if _WIN32
#endif

// =====================================================

configParserStruct::mutex::mutex() : 
  Mutex(NULL)
{
#if __unix__
  Mutex = new pthread_mutex_t();
  pthread_mutex_init( castToMutex(Mutex), NULL );
#endif
}

// -----------------------------------------------------

configParserStruct::mutex::~mutex()
{

#if __unix__
  if ( Mutex != NULL )
    pthread_mutex_destroy( castToMutex(Mutex) );
#endif

  delete castToMutex(Mutex);
  Mutex = NULL;
}

// -----------------------------------------------------

void configParserStruct::mutex::lock()
{
  assert( Mutex != NULL );
#if __unix__
  pthread_mutex_lock( castToMutex(Mutex) );
#endif
}

// -----------------------------------------------------

void configParserStruct::mutex::unlock()
{
  assert( Mutex != NULL );
#if __unix__
  pthread_mutex_unlock( castToMutex(Mutex) );
#endif
}

// =====================================================

