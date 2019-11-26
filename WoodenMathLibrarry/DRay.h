#pragma once
#include "stdafx.h"
#include "DVector.h"
#include "DPoint.h"

WML_BEGIN

template<typename T>
class ALIGN_AS_VEC(T, 3) DRay
{
public:
	DRay()
	{};


	DRay(DPoint<T, 3> o, DVector<T, 3> d):
		origin(std::move(o)),
		dir(std::move(d))
	{}

	inline DPoint<T, 3> operator()(T t) const
	{
		return mad(dir, t, origin);
	}

	DPoint<T, 3> origin;
	DVector<T, 3> dir;
	float t;
};



using DRayf = typename DRay<float>;
using DRayd = typename DRay<double>;

WML_END