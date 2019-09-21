// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#define _USE_MATH_DEFINES

#include "gtest/gtest.h"
#include <math.h>


//#define ASSERT_VFLOAT3_EQ(v1, v2) do{\
//	ASSERT_FLOAT_EQ(v1.x, v2.x);\
//	ASSERT_FLOAT_EQ(v1.y, v2.y);\
//	ASSERT_FLOAT_EQ(v1.z, v2.z);\
//}while(false);



#define ASSERT_VINT3_EQ(v1, v2x, v2y, v2z) do{\
	ASSERT_EQ(v1.x(), v2x);\
	ASSERT_EQ(v1.y(), v2y);\
	ASSERT_EQ(v1.z(), v2z);\
}while(false);


#define ASSERT_VFLOAT3_EQ(v1, v2x, v2y, v2z) do{\
	ASSERT_FLOAT_EQ(v1.x(), v2x);\
	ASSERT_FLOAT_EQ(v1.y(), v2y);\
	ASSERT_FLOAT_EQ(v1.z(), v2z);\
}while(false);

#define ASSERT_VFLOAT4_EQ(v1, v2x, v2y, v2z, v2w) do{\
	ASSERT_FLOAT_EQ(v1.x(), v2x);\
	ASSERT_FLOAT_EQ(v1.y(), v2y);\
	ASSERT_FLOAT_EQ(v1.z(), v2z);\
	ASSERT_FLOAT_EQ(v1.w(), v2w);\
}while(false);


#define ASSERT_MATRIX_EQ(m, m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) do{\
ASSERT_FLOAT_EQ(m[0][0], m00); ASSERT_FLOAT_EQ(m[0][1], m10); ASSERT_FLOAT_EQ(m[0][2], m20); ASSERT_FLOAT_EQ(m[0][3], m30); \
ASSERT_FLOAT_EQ(m[1][0], m01); ASSERT_FLOAT_EQ(m[1][1], m11); ASSERT_FLOAT_EQ(m[1][2], m21); ASSERT_FLOAT_EQ(m[1][3], m31); \
ASSERT_FLOAT_EQ(m[2][0], m02); ASSERT_FLOAT_EQ(m[2][1], m12); ASSERT_FLOAT_EQ(m[2][2], m22); ASSERT_FLOAT_EQ(m[2][3], m32); \
ASSERT_FLOAT_EQ(m[3][0], m03); ASSERT_FLOAT_EQ(m[3][1], m13); ASSERT_FLOAT_EQ(m[3][2], m23); ASSERT_FLOAT_EQ(m[3][3], m33); \
}while(false);

// TODO: add headers that you want to pre-compile here

#endif //PCH_H
