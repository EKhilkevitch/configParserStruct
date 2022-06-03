
#pragma once

// =====================================================

namespace configParserStruct
{

  // =====================================================

  class mutex
  {
    private:
      struct impl;

    private:
      impl *Impl;

    private:
      mutex( const mutex& );
      mutex& operator=( const mutex& );

    public:
      mutex();
      ~mutex();

      void lock();
      void unlock();
  };
  
  // =====================================================
  
  class mutexLocker
  {
    private:
      mutex *Mutex;

    private:
      mutexLocker( const mutexLocker& );
      mutexLocker& operator=( const mutexLocker& );

    public:
      mutexLocker( mutex *M );
      ~mutexLocker();
  };
  
  // =====================================================

}

// =====================================================

