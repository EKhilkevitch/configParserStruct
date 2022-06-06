
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

// ---------------------------------------------------------

TEST( variable, dict_keys )
{
  variable Dict( variable::DictCollection );
  Dict.setByRef( reference( "a", reference::DictKey ), variable( 1 ) );
  Dict.setByRef( reference( "b", reference::DictKey ), variable( 3 ) );
  Dict.setByRef( reference( "c", reference::DictKey ), variable( 5 ) );

  EXPECT_EQ( "{ .a = 1, .b = 3, .c = 5 }", Dict.string() );

  const variable *Keys = Dict.getByRef( reference( dictVariableValue::ArrayOfKeysName, reference::DictKey ) );
  ASSERT_TRUE( Keys != NULL );
  EXPECT_EQ( "[ a, b, c ]", Keys->string() );
}

// =========================================================

