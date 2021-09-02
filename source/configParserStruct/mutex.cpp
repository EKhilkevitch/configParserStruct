
// =====================================================

#include "configParserStruct/mutex.h"

#include <cassert>
#include <cstdlib>

#if __unix__
#  include <pthread.h>
#endif

#if _WIN32
#  include <windows.h>
#endif

// =====================================================

struct configParserStruct::mutex::impl
{
#if __unix__
  pthread_mutex_t Mutex;
#elif _WIN32
  HANDLE Mutex;
#else
#  error "Not implemented"
#endif

  impl() {}

  private:
    impl( const impl& );
    impl& operator=( const impl& );
};

// =====================================================

configParserStruct::mutex::mutex() : 
  Impl( new impl() )
{
#if __unix__
  pthread_mutex_init( &Impl->Mutex, NULL );
#endif
#if _WIN32
  Impl->Mutex = CreateMutex( NULL, FALSE, NULL );
#endif
}

// -----------------------------------------------------

configParserStruct::mutex::~mutex()
{
#if __unix__
  pthread_mutex_destroy( &Impl->Mutex );
#endif
#if _WIN32
  if ( Impl->Mutex != INVALID_HANDLE_VALUE )
    CloseHandle( Impl->Mutex );
#endif

  delete Impl;
}

// -----------------------------------------------------

void configParserStruct::mutex::lock()
{
#if __unix__
  pthread_mutex_lock( &Impl->Mutex );
#endif
#if _WIN32
  WaitForSingleObject( Impl->Mutex, INFINITE );
#endif
}

// -----------------------------------------------------

void configParserStruct::mutex::unlock()
{
#if __unix__
  pthread_mutex_unlock( &Impl->Mutex );
#endif
#if _WIN32
  ReleaseMutex( Impl->Mutex ); 
#endif
}

// =====================================================

configParserStruct::mutexLocker::mutexLocker( mutex *M ) :
  Mutex( M )
{
  if ( Mutex != NULL )
    Mutex->lock();
}

// -----------------------------------------------------
      
configParserStruct::mutexLocker::~mutexLocker()
{
  if ( Mutex != NULL )
    Mutex->unlock();
  Mutex = NULL;
}

// =====================================================

