
#pragma once
#include "pch.h"
#include "WoodenMathLibrarry/DVector.h"
#include "WoodenMathLibrarry/DPoint.h"
#include "WoodenMathLibrarry/DNormal.h"

using namespace wml;


namespace
{
	using VecT = typename DNormal3f;

	TEST(VectorF, Init)
	{
		VecT v;
		ASSERT_VFLOAT3_EQ(v, 0.0f, 0.0f, 0.0f);

		v = VecT(1.0f, 2.0f, 3.0f);
		ASSERT_VFLOAT3_EQ(v, 1.0f, 2.0f, 3.0f);
	}

	TEST(VectorF, ConvertToInt)
	{
		const VecT v(10.0f, 12.5f, 13.1f);
		DVector3i a = v;
		ASSERT_VINT3_EQ(a, 10, 12, 13);
	}

	TEST(VectorF, Addition)
	{
		VecT v1(0.0f, 1.0f, 4.0f);
		VecT v2(1.0f, 2.0f, 3.0f);

		VecT v3 = v1 + v2;

		ASSERT_VFLOAT3_EQ(v3, 1.0f, 3.0f, 7.0f);
		v3 += v3;
		ASSERT_VFLOAT3_EQ(v3, 2.0f, 6.0f, 14.0f);
	}

	TEST(VectorF, Substraction)
	{
		VecT v1(0.0f, 1.0f, 4.0f);
		VecT v2(1.0f, 2.0f, 3.0f);

		VecT v3 = v1 - v2;

		ASSERT_VFLOAT3_EQ(v3, -1.0f, -1.0f, 1.0f);
		v3 -= v3;
		ASSERT_VFLOAT3_EQ(v3, 0.0f, 0.0f, 0.0f);
	}


	TEST(VectorF, ScalarMulti)
	{
		VecT v1(0.0f, 1.0f, 4.0f);
		float s = 3.0f;

		VecT v2 = v1*s;
		
		ASSERT_VFLOAT3_EQ(v2, v1.x()*s, v1.y()*s, v1.z()*s);

		s = 10.0f;
		
		VecT v2tmp = v2;

		v2 *= s;
		ASSERT_VFLOAT3_EQ(v2, v2tmp.x()*s, v2tmp.y()*s, v2tmp.z()*s);

		v2tmp = v2;

		int32_t s2 = 20;
		v2 *= s2;

		ASSERT_VFLOAT3_EQ(v2, v2tmp.x()*s2, v2tmp.y()*s2, v2tmp.z()*s2);
	}

	TEST(VectorF, PerElementMultiplication)
	{
		VecT v1(0.0f, 1.0f, 4.0f);
		VecT v2(1.0f, 2.0f, 3.0f);

		VecT v3 = v1 * v2;

		ASSERT_VFLOAT3_EQ(v3, 0.0f, 2.0f, 12.0f);
		v3 *= v3;
		ASSERT_VFLOAT3_EQ(v3, 0.0f, 4.0f, 144.0f);
	}

	TEST(VectorF, PerElementDivision)
	{
		VecT v1(0.0f, 1.0f, 4.0f);
		VecT v2(1.0f, 2.0f, 3.0f);

		VecT v3 = v1/ v2;

		ASSERT_VFLOAT3_EQ(v3, 0.0f, 1.0f/2.0f, 4.0f/3.0f);
	}

	TEST(VectorF, Normalize)
	{
		VecT v1(0.5f, 1.0f, 4.0f);
		VecT vNormalize = normalize(v1);
		ASSERT_VFLOAT3_EQ(vNormalize, 0.12038585308576920076209076441895f, 0.2407717061715384015241815288379f, 0.9630868246861536060967261153516f);
	}

	TEST(VectorF, Length)
	{
		VecT v1(2.0f, 1.0f, 4.0f);
		ASSERT_FLOAT_EQ(v1.length2(), 21.0f);
		ASSERT_FLOAT_EQ(v1.length(), sqrt(21.0f));
	}

	TEST(VectorF, Lerp)
	{
		VecT v1(2.0f, 1.0f, 4.0f);
		VecT v2(4.0f, 2.0f, 6.0f);
		float t = 0.4f;
		float tInv = 1.0f - t;

		ASSERT_VFLOAT3_EQ(lerp(v1, v2, t), tInv*v1.x() + t * v2.x(),
						  tInv*v1.y() + t * v2.y(), tInv*v1.z() + t * v2.z());
	}


	TEST(VectorF, Dot)
	{
		VecT v1(0.0f, 1.0f, 4.0f);
		VecT v2(10.0f, 20.0f, 5.0f);

		float dotv = dot(v1, v2);
		ASSERT_FLOAT_EQ(dotv, v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z());
	}

	TEST(VectorF, Cross)
	{
		VecT v1(1.0f, 1.0f, 4.0f);
		VecT v2(10.0f, 20.0f, 5.0f);

		VecT crossv = cross(v1, v2);
		ASSERT_VFLOAT3_EQ(crossv, -75.0f, 35.0f, 10.0f);
	}

	TEST(VectorF, Min)
	{
		VecT v1(2.0f, 1.0f, 20.0f);
		ASSERT_FLOAT_EQ(minComponent(v1), 1.0f);

		VecT v2(10.0f, 20.0f, 5.0f);
		ASSERT_VFLOAT3_EQ(minv(v1, v2), 2.0f, 1.0f, 5.0f);
	}

	TEST(VectorF, Equality)
	{
		VecT v1(2.0f, 1.0f, 20.0f);
		VecT v2(2.0f, 1.0f, 20.0f);

		ASSERT_EQ(v1, v2);
	}


}