
#pragma once
#include "pch.h"
#include "WoodenMathLibrarry/DVector.h"

using namespace wml;

//#define ASSERT_VFLOAT3_EQ(v1, v2) do{\
//	ASSERT_FLOAT_EQ(v1.x, v2.x);\
//	ASSERT_FLOAT_EQ(v1.y, v2.y);\
//	ASSERT_FLOAT_EQ(v1.z, v2.z);\
//}while(false);

#define ASSERT_VFLOAT3_EQ(v1, v2x, v2y, v2z) do{\
	ASSERT_FLOAT_EQ(v1.x(), v2x);\
	ASSERT_FLOAT_EQ(v1.y(), v2y);\
	ASSERT_FLOAT_EQ(v1.z(), v2z);\
}while(false);

namespace
{
	TEST(VectorF, Init)
	{
		DVector3f v;
		ASSERT_VFLOAT3_EQ(v, 0.0f, 0.0f, 0.0f);

		v = DVector3f(1.0f, 2.0f, 3.0f);
		ASSERT_VFLOAT3_EQ(v, 1.0f, 2.0f, 3.0f);
	}

	TEST(VectorF, Addition)
	{
		DVector3f v1(0.0f, 1.0f, 4.0f);
		DVector3f v2(1.0f, 2.0f, 3.0f);

		DVector3f v3 = v1 + v2;

		ASSERT_VFLOAT3_EQ(v3, 1.0f, 3.0f, 7.0f);
		v3 += v3;
		ASSERT_VFLOAT3_EQ(v3, 2.0f, 6.0f, 14.0f);
	}

	TEST(VectorF, Substraction)
	{
		DVector3f v1(0.0f, 1.0f, 4.0f);
		DVector3f v2(1.0f, 2.0f, 3.0f);

		DVector3f v3 = v1 - v2;

		ASSERT_VFLOAT3_EQ(v3, -1.0f, -1.0f, 1.0f);
		v3 -= v3;
		ASSERT_VFLOAT3_EQ(v3, 0.0f, 0.0f, 0.0f);
	}


	TEST(VectorF, ScalarMulti)
	{
		DVector3f v1(0.0f, 1.0f, 4.0f);
		float s = 3.0f;

		DVector3f v2 = v1*s;
		
		ASSERT_VFLOAT3_EQ(v2, v1.x()*s, v1.y()*s, v1.z()*s);

		s = 10.0f;
		
		DVector3f v2tmp = v2;

		v2 *= s;
		ASSERT_VFLOAT3_EQ(v2, v2tmp.x()*s, v2tmp.y()*s, v2tmp.z()*s);

		v2tmp = v2;

		int32_t s2 = 20;
		v2 *= s2;

		ASSERT_VFLOAT3_EQ(v2, v2tmp.x()*s2, v2tmp.y()*s2, v2tmp.z()*s2);
	}

	TEST(VectorF, PerElementMultiplication)
	{
		DVector3f v1(0.0f, 1.0f, 4.0f);
		DVector3f v2(1.0f, 2.0f, 3.0f);

		DVector3f v3 = v1 * v2;

		ASSERT_VFLOAT3_EQ(v3, 0.0f, 2.0f, 12.0f);
		v3 *= v3;
		ASSERT_VFLOAT3_EQ(v3, 0.0f, 4.0f, 144.0f);
	}

	TEST(VectorF, PerElementDivision)
	{
		DVector3f v1(0.0f, 1.0f, 4.0f);
		DVector3f v2(1.0f, 2.0f, 3.0f);

		DVector3f v3 = v1/ v2;

		ASSERT_VFLOAT3_EQ(v3, 0.0f, 1.0f/2.0f, 4.0f/3.0f);
	}

	TEST(VectorF, Normalize)
	{
		DVector3f v1(0.5f, 1.0f, 4.0f);
		DVector3f vNormalize = DVector3f::normalize(v1);
		ASSERT_VFLOAT3_EQ(vNormalize, 0.12038585308576920076209076441895f, 0.2407717061715384015241815288379f, 0.9630868246861536060967261153516f);
	}

	TEST(VectorF, Length)
	{
		DVector3f v1(2.0f, 1.0f, 4.0f);
		ASSERT_FLOAT_EQ(v1.length2(), 21.0f);
		ASSERT_FLOAT_EQ(v1.length(), sqrt(21.0f));
	}

	TEST(VectorF, Lerp)
	{
		DVector3f v1(2.0f, 1.0f, 4.0f);
		DVector3f v2(4.0f, 2.0f, 6.0f);
		float t = 0.4f;
		float tInv = 1.0f - t;

		ASSERT_VFLOAT3_EQ(DVector3f::lerp(v1, v2, t), tInv*v1.x() + t * v2.x(),
						  tInv*v1.y() + t * v2.y(), tInv*v1.z() + t * v2.z());
	}


	TEST(VectorF, Dot)
	{
		DVector3f v1(0.0f, 1.0f, 4.0f);
		DVector3f v2(10.0f, 20.0f, 5.0f);

		float dot = DVector3f::dot(v1, v2);
		ASSERT_FLOAT_EQ(dot, v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z());
	}

	TEST(VectorF, Cross)
	{
		DVector3f v1(1.0f, 1.0f, 4.0f);
		DVector3f v2(10.0f, 20.0f, 5.0f);

		DVector3f cross = DVector3f::cross(v1, v2);
		ASSERT_VFLOAT3_EQ(cross, -75.0f, 35.0f, 10.0f);
	}

	TEST(VectorF, Min)
	{
		DVector3f v1(2.0f, 1.0f, 20.0f);
		ASSERT_FLOAT_EQ(v1.minComponent(), 1.0f);

		DVector3f v2(10.0f, 20.0f, 5.0f);
		ASSERT_VFLOAT3_EQ(DVector3f::minVector(v1, v2), 2.0f, 1.0f, 5.0f);
	}

	TEST(VectorF, Equality)
	{
		DVector3f v1(2.0f, 1.0f, 20.0f);
		DVector3f v2(2.0f, 1.0f, 20.0f);

		ASSERT_EQ(v1, v2);
	}
}