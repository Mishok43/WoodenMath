
#pragma once
#include "pch.h"
#include "WoodenMathLibrarry/DVector.h"

namespace
{
	TEST(Vector, Init)
	{
		DVector3f v;
		EXPECT_EQ(v.x, 0.0f);
		EXPECT_EQ(v.y, 0.0f);
		EXPECT_EQ(v.z, 0.0f);

		v = DVector3f(1.0f, 2.0f, 3.0f);
		ASSERT_EQ(v.x, 1.0f);
		ASSERT_EQ(v.y, 2.0f);
		ASSERT_EQ(v.z, 3.0f);
	}
}