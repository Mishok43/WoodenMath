
#pragma once
#include "pch.h"
#include "WoodenMathLibrarry/DRay.h"

using namespace wml;


namespace
{
	using RayT = typename DRayf;

	TEST(Ray, Init)
	{
		RayT r(DPoint3f(0.0f, 1.0f, 3.0), DVector3f(1.0f, 0.0f, 0.0f));


		DPoint3f p = r(5.0f);
		ASSERT_VFLOAT3_EQ(p, 5.0f, 1.0f, 3.0f);
	}


}