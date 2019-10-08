#pragma once
#include "stdafx.h"
#include "DVector.h"
#include "DPoint.h"

WML_BEGIN

template<typename T, typename __mT = __m_t<T>, uint8_t alignment = sse_alignment_size_v<__mT>>
class alignas(alignment) DRay
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