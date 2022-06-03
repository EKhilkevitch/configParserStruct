
#pragma once

// =====================================================

#include <cstddef>

// =====================================================

namespace configParserStruct
{

  // =====================================================
    
  template < class T > class clonablePointer 
  {
    private:
      T *Pointer;

    public:
      clonablePointer();
      explicit clonablePointer( T *Ptr );
      clonablePointer( const clonablePointer &Arg );
      clonablePointer& operator=( const clonablePointer &Arg );
      ~clonablePointer();

      T& operator*()              { return *Pointer; }
      const T& operator*() const  { return *Pointer; }
      T* operator->()             { return Pointer; }
      const T* operator->() const { return Pointer; }

      const T* get() const { return Pointer; }
      T* get()             { return Pointer; }
  };
    
  // =====================================================
  
  template <class T> clonablePointer<T>::clonablePointer() :
    Pointer( NULL )
  {
  }
  
  // -----------------------------------------------------
  
  template <class T> clonablePointer<T>::clonablePointer( T *Ptr ) :
    Pointer(Ptr)
  {
  }

  // -----------------------------------------------------
  
  template <class T> clonablePointer<T>::clonablePointer( const clonablePointer &Arg ) :
    Pointer(NULL)
  {
    if ( Arg.Pointer != NULL )
      Pointer = Arg.Pointer->clone();
  }

  // -----------------------------------------------------

  template <class T> clonablePointer<T>& clonablePointer<T>::operator=( const clonablePointer &Arg )
  {
    if ( &Arg == this )
      return *this;

    if ( Arg.Pointer == NULL )
    {
      delete Pointer;
      Pointer = NULL;
      return *this;
    }

    T *Clone = Arg.Pointer->clone();
    delete Pointer;
    Pointer = Clone;

    return *this;
  }
  
  // -----------------------------------------------------

  template <class T> clonablePointer<T>::~clonablePointer()
  {
    delete Pointer;
  }

  // =====================================================

}

