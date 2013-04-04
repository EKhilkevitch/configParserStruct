
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
  Mutex = new pthread_mutex_t();
  pthread_mutex_init( castToMutex(Mutex), NULL );
}

// -----------------------------------------------------

configParserStruct::mutex::~mutex()
{
  if ( Mutex != NULL )
    pthread_mutex_destroy( castToMutex(Mutex) );
  delete castToMutex(Mutex);
  Mutex = NULL;
}

// -----------------------------------------------------

void configParserStruct::mutex::lock()
{
  assert( Mutex != NULL );
  pthread_mutex_lock( castToMutex(Mutex) );
}

// -----------------------------------------------------

void configParserStruct::mutex::unlock()
{
  assert( Mutex != NULL );
  pthread_mutex_unlock( castToMutex(Mutex) );
}

// =====================================================

