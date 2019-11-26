#pragma once
#include "pch.h"
#include "WoodenMathLibrarry/DQuaternion.h"


using namespace wml;
namespace
{

	TEST(QuaternionF, Init)
	{
		DQuaternionf q = DQuaternionf(1.0f, 0.0f, 0.0f, 3.14/4.0);
		ASSERT_VFLOAT4_EQ(q, 0.3824995, 0.0f, 0.0f, 0.9239557);
	}


	TEST(QuaternionF, Mult)
	{
		DQuaternionf q0 = DQuaternionf(1.0f, 0.0f, 0.0f, 3.14 / 4.0);
		DQuaternionf q2 = DQuaternionf(1.0f, 0.0f, 0.0f, 3.14 / 2.0);
/*
		DQuaternionf q3 = q0 * q2;
		ASSERT_VFLOAT4_EQ(q3, 0.9236508, 0.0f, 0.0f, 0.3832351);*/
	}

	TEST(QuaternionF, MultVector)
	{
		DQuaternionf q = DQuaternionf(0.0f, 0.0f, 1.0f, M_PI_2);
		DVector4f v = DVector4f(1.0f, 0.0f, 0.0f, 1.0f);
		v = v * q;
		ASSERT_VFLOAT4_EQ(v, 0.0, 1.0f, 0.0f, 1.0);
	}

	TEST(QuaternionF, RotMatrix)
	{
		DQuaternionf q0 = DQuaternionf(1.0f, 0.0f, 0.0f, -3.14 / 4.0);
		DQuaternionf q2 = DQuaternionf(1.0f, 0.0f, 0.0f, -3.14 / 2.0);

		DQuaternionf q3 = q0 * q2;

		DMatrixf m = makeMatrix(q3);
		ASSERT_MATRIX_EQ(m,
						1.0000000, 0.0000000, 0.0000000, 0.0,
						0.0000000, -0.7062616, -0.7079509, 0.0,
						0.0000000, 0.7079509, -0.7062616, 0.0,
						0.0, 0.0, 0.0, 1.0);
	}


}
