
// =========================================================

#include "configParserStruct/variable.h"
#include "configParserStruct/variablevalue.h"
#include "configParserStruct/reference.h"

#include <gtest/gtest.h>

using namespace configParserStruct;

// =========================================================

TEST( variable, undef )
{
  variable Variable;

  EXPECT_EQ( 0, Variable.integer() );
  EXPECT_EQ( 0.0, Variable.real() );
  EXPECT_EQ( std::string(), Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, integer )
{
  variable Variable(5);

  EXPECT_EQ( 5, Variable.integer() );
  EXPECT_EQ( 5.0, Variable.real() );
  EXPECT_EQ( "5", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, real )
{
  variable Variable(5.5);

  EXPECT_EQ( 5,   Variable.integer() );
  EXPECT_EQ( 5.5, Variable.real() );
  EXPECT_EQ( "5.5", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, string )
{
  variable Variable( "abc" );
  EXPECT_EQ( 0, Variable.integer() );
  EXPECT_EQ( 0.0, Variable.real() );
  EXPECT_EQ( "abc", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, array_1 )
{
  variable Variable( variable::ArrayCollection );
  Variable.setByRef( reference( std::numeric_limits<size_t>::max(), reference::ArrayIndex ), variable(5) );
  Variable.setByRef( reference( std::numeric_limits<size_t>::max(), reference::ArrayIndex ), variable(6) );
  Variable.setByRef( reference( std::numeric_limits<size_t>::max(), reference::ArrayIndex ), variable(7) );

  ASSERT_TRUE( NULL != Variable.getByRef( reference( static_cast<size_t>(0), reference::ArrayIndex ) ) );
  EXPECT_EQ( 5, Variable.getByRef( reference( static_cast<size_t>(0), reference::ArrayIndex ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( 1, reference::ArrayIndex ) ) );
  EXPECT_EQ( 6, Variable.getByRef( reference( 1, reference::ArrayIndex ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( 2, reference::ArrayIndex ) ) );
  EXPECT_EQ( 7, Variable.getByRef( reference( 2, reference::ArrayIndex ) )->integer() );
  
  ASSERT_TRUE( NULL == Variable.getByRef( reference( std::numeric_limits<size_t>::max(), reference::ArrayIndex ) ) );

  EXPECT_EQ( 3, Variable.integer() );
  EXPECT_EQ( "[ 5, 6, 7 ]", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, array_2 )
{
  variable Variable( variable::ArrayCollection );
  Variable.setByRef( reference( 3, reference::ArrayIndex ), variable(5) );
  Variable.setByRef( reference( 4, reference::ArrayIndex ), variable(6) );
  Variable.setByRef( reference( 6, reference::ArrayIndex ), variable(7) );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( static_cast<size_t>(0), reference::ArrayIndex ) ) );
  EXPECT_EQ( 0, Variable.getByRef( reference( static_cast<size_t>(0), reference::ArrayIndex ) )->integer() );

  ASSERT_TRUE( NULL != Variable.getByRef( reference( 3, reference::ArrayIndex ) ) );
  EXPECT_EQ( 5, Variable.getByRef( reference( 3, reference::ArrayIndex ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( 4, reference::ArrayIndex ) ) );
  EXPECT_EQ( 6, Variable.getByRef( reference( 4, reference::ArrayIndex ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( 6, reference::ArrayIndex ) ) );
  EXPECT_EQ( 7, Variable.getByRef( reference( 6, reference::ArrayIndex ) )->integer() );
  
  EXPECT_EQ( 7, Variable.integer() );
  EXPECT_EQ( "[ , , , 5, 6, , 7 ]", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, array_3 )
{
  variable Variable( variable::ArrayCollection );
  Variable.setByRef( reference( 3, reference::ArrayIndex ), variable(5) );
  Variable.setByRef( reference( 3, reference::ArrayIndex ), variable(7) );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( 3, reference::ArrayIndex ) ) );
  EXPECT_EQ( 7, Variable.getByRef( reference( 3, reference::ArrayIndex ) )->integer() );
}

// ---------------------------------------------------------

TEST( variable, dict_1 )
{
  variable Variable( variable::DictCollection );
  Variable.setByRef( reference( "a", reference::DictKey ), variable( 1 ) );
  Variable.setByRef( reference( "b", reference::DictKey ), variable( 3 ) );
  Variable.setByRef( reference( "c", reference::DictKey ), variable( 5 ) );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( "a", reference::DictKey ) ) );
  EXPECT_EQ( 1, Variable.getByRef( reference( "a", reference::DictKey ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( "b", reference::DictKey ) ) );
  EXPECT_EQ( 3, Variable.getByRef( reference( "b", reference::DictKey ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( "c", reference::DictKey ) ) );
  EXPECT_EQ( 5, Variable.getByRef( reference( "c", reference::DictKey ) )->integer() );
   
  EXPECT_EQ( 3, Variable.integer() );
  EXPECT_EQ( "{ .a = 1, .b = 3, .c = 5 }", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, dict_2 )
{
  variable Variable( variable::DictCollection );
  Variable.setByRef( reference( "a", reference::DictKey ), variable( 1 ) );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( "a", reference::DictKey ) ) );
  EXPECT_EQ( 1, Variable.getByRef( reference( "a", reference::DictKey ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( "b", reference::DictKey ) ) );
  EXPECT_EQ( 0, Variable.getByRef( reference( "b", reference::DictKey ) )->integer() );
  
  EXPECT_EQ( 2, Variable.integer() );
  EXPECT_EQ( "{ .a = 1, .b =  }", Variable.string() );
}

// ---------------------------------------------------------

TEST( variable, dict_3 )
{
  variable Variable( variable::DictCollection );
  Variable.setByRef( reference( "a", reference::DictKey ), variable( 1 ) );
  Variable.setByRef( reference( "b", reference::DictKey ), variable( 3 ) );
  Variable.setByRef( reference( "a", reference::DictKey ), variable( 5 ) );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( "a", reference::DictKey ) ) );
  EXPECT_EQ( 5, Variable.getByRef( reference( "a", reference::DictKey ) )->integer() );
  
  ASSERT_TRUE( NULL != Variable.getByRef( reference( "b", reference::DictKey ) ) );
  EXPECT_EQ( 3, Variable.getByRef( reference( "b", reference::DictKey ) )->integer() );
}

// ---------------------------------------------------------

TEST( variable, dict_keys )
{
  variable Dict( variable::DictCollection );
  Dict.setByRef( reference( "a", reference::DictKey ), variable( 1 ) );
  Dict.setByRef( reference( "b", reference::DictKey ), variable( 3 ) );
  Dict.setByRef( reference( "c", reference::DictKey ), variable( 5 ) );

  const variable *Keys = Dict.getByRef( reference( dictVariableValue::ArrayOfKeysName, reference::DictKey ) );
  ASSERT_TRUE( Keys != NULL );
  EXPECT_EQ( "[ a, b, c ]", Keys->string() );
}

// ---------------------------------------------------------

TEST( variable, constructCopy )
{
  variable Src(5);
  variable Dst(Src);

  EXPECT_EQ( 5, Dst.integer() );
  EXPECT_EQ( 5.0, Dst.real() );
  EXPECT_EQ( "5", Dst.string() );
}

// ---------------------------------------------------------

TEST( variable, copy )
{
  variable Src(5);
  variable Dst(3);

  Dst = Src;

  EXPECT_EQ( 5, Dst.integer() );
  EXPECT_EQ( 5.0, Dst.real() );
  EXPECT_EQ( "5", Dst.string() );
}

// =========================================================

