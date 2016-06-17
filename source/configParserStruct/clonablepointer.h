
#pragma once

// =====================================================

#include <cassert>
#include <typeinfo>

// =====================================================

namespace configParserStruct
{

  // =====================================================
    
  template <class T, class DefaultT> class clonablePointer 
  {
    private:
      T *Pointer;

    public:
      clonablePointer();
      explicit clonablePointer( const T &Arg );
      explicit clonablePointer( T *Ptr );
      clonablePointer( const clonablePointer &Arg );
      clonablePointer& operator=( const clonablePointer &Arg );
      ~clonablePointer();

      T& operator*()              { return *Pointer; }
      const T& operator*() const  { return *Pointer; }
      T* operator->()             { return Pointer; }
      const T* operator->() const { return Pointer; }

      const std::type_info& type() const;
      template <class X> bool isDerivedFrom() const;
  };
    
  // =====================================================
  
  template <class T, class DefaultT> clonablePointer<T,DefaultT>::clonablePointer() :
    Pointer(NULL)
  {
    Pointer = new DefaultT();
  }
  
  // -----------------------------------------------------

  template <class T, class DefaultT> clonablePointer<T,DefaultT>::clonablePointer( const T &Arg ) :
    Pointer(NULL)
  {
    Pointer = Arg.clone();
    assert( Pointer != NULL );
    assert( type() == typeid(Arg) );
  }
  
  // -----------------------------------------------------
  
  template <class T, class DefaultT> clonablePointer<T,DefaultT>::clonablePointer( T *Ptr ) :
    Pointer(Ptr)
  {
    assert( Pointer != NULL );
  }

  // -----------------------------------------------------
  
  template <class T, class DefaultT> clonablePointer<T,DefaultT>::clonablePointer( const clonablePointer &Arg ) :
    Pointer(NULL)
  {
    *this = Arg;
    assert( Pointer != NULL );
  }

  // -----------------------------------------------------

  template <class T, class DefaultT> clonablePointer<T,DefaultT>& clonablePointer<T,DefaultT>::operator=( const clonablePointer &Arg )
  {
    if ( &Arg == this )
      return *this;

    assert( Arg.Pointer != NULL );
    assert( Arg.Pointer != Pointer );

    T *Clone = Arg.Pointer->clone();

    delete Pointer;
    Pointer = Clone;

    assert( type() == Arg.type() );
   
    return *this;
  }
        
  // -----------------------------------------------------
  
  template <class T, class DefaultT> const std::type_info& clonablePointer<T,DefaultT>::type() const 
  { 
    return typeid(*Pointer); 
  }
  
  // -----------------------------------------------------
  
  template <class T, class DefaultT> template <class X> bool clonablePointer<T,DefaultT>::isDerivedFrom() const 
  { 
    return dynamic_cast<const X*>(Pointer) != NULL; 
  }
  
  // -----------------------------------------------------

  template <class T, class DefaultT> clonablePointer<T,DefaultT>::~clonablePointer()
  {
    delete Pointer;
  }

  // =====================================================

}

