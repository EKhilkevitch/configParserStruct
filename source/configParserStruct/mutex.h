
#pragma once

// =====================================================

namespace configParserStruct
{

  class mutex
  {
    private:
      void *Mutex;

    private:
      mutex( const mutex& );
      mutex& operator=( const mutex& );

    public:
      mutex();
      ~mutex();

      void lock();
      void unlock();
  };

}

// =====================================================


