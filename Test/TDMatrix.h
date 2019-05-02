#pragma once
#include "pch.h"
#include "WoodenMathLibrarry/DMatrix.h"

using namespace wml;

namespace
{
	TEST(MatrixF, Init)
	{
		DMatrixf m1;
		ASSERT_MATRIX_EQ(m1,
						 1.0, 0.0, 0.0, 0.0,
						 0.0, 1.0, 0.0, 0.0,
						 0.0, 0.0, 1.0, 0.0,
						 0.0, 0.0, 0.0, 1.0);
	}

	TEST(MatrixF, Addition)
	{
		DMatrixf m1, m2;
		m1[0][0] = 4.0f;
		m1[2][1] = 10.0f;

		DMatrixf m3 = m1 + m2;
		ASSERT_MATRIX_EQ(m3,
						 5.0, 0.0, 0.0, 0.0,
						 0.0, 2.0, 10.0, 0.0,
						 0.0, 0.0, 2.0, 0.0,
						 0.0, 0.0, 0.0, 2.0);
	}

	TEST(MatrixF, Substraction)
	{
		DMatrixf m1, m2;
		m1[0][0] = 4.0f;
		m1[2][1] = 10.0f;

		DMatrixf m3 = m1 - m2;
		ASSERT_MATRIX_EQ(m3,
						 3.0, 0.0, 0.0, 0.0,
						 0.0, 0.0, 10.0, 0.0,
						 0.0, 0.0, 0.0, 0.0,
						 0.0, 0.0, 0.0, 0.0);
	}

	TEST(MatrixF, MultMatrix)
	{
		DMatrixf m1, m2;
		m1[0][0] = 4.0f;
		m1[2][1] = 10.0f;
		m2[2][1] = 3.0f;
		m2[1][1] = 6.0f;

		DMatrixf m3 = m1 * m2;
		ASSERT_MATRIX_EQ(m3,
						 4.0, 0.0, 0.0, 0.0,
						 0.0, 6.0, 13.0, 0.0,
						 0.0, 0.0, 1.0, 0.0,
						 0.0, 0.0, 0.0, 1.0);
	}

	TEST(MatrixF, MultScalar)
	{
		DMatrixf m1;
		m1[0][0] = 4.0f;
		m1[2][1] = 10.0f;

		DMatrixf m3 = m1 * 5.0f;
		ASSERT_MATRIX_EQ(m3,
						 20.0, 0.0, 0.0, 0.0,
						 0.0, 5.0, 50.0, 0.0,
						 0.0, 0.0, 5.0, 0.0,
						 0.0, 0.0, 0.0, 5.0);
	}

	TEST(MatrixF, MultVector)
	{
		DMatrixf m1;
		m1[0][0] = 4.0f;

		DVector3f v1(1.0f, 2.0f, 3.0f);
		v1 = v1 * m1;
		ASSERT_VFLOAT3_EQ(v1, 4.0f, 2.0f, 3.0f);
	}

	TEST(MatrixF, Transpose)
	{
		DMatrixf m1;
		m1[0][0] = 4.0f;
		m1[2][1] = 10.0f;

		m1 = DMatrixf::transpose(m1);
		ASSERT_MATRIX_EQ(m1,
						 4.0, 0.0, 0.0, 0.0,
						 0.0, 1.0, 0.0, 0.0,
						 0.0, 10.0, 1.0, 0.0,
						 0.0, 0.0, 0.0, 1.0);
	}

	TEST(MatrixD, MultMatrix)
	{
		DMatrixd m1, m2;
		m1[0][0] = 4.0;
		m1[2][1] = 10.0;
		m2[2][1] = 3.0;
		m2[1][1] = 6.0;

		DMatrixd m3 = m1 * m2;
		ASSERT_MATRIX_EQ(m3,
						 4.0, 0.0, 0.0, 0.0,
						 0.0, 6.0, 13.0, 0.0,
						 0.0, 0.0, 1.0, 0.0,
						 0.0, 0.0, 0.0, 1.0);
	}
}