#pragma once

#include "stdafx.h"
#include "DVector.h"

WML_BEGIN

using namespace TypedSSE;

template<typename T, uint8_t Size, uint8_t VSize = Size+1>
class DPoint : public DVector<T, VSize>
{
public:
	operator DVector<T, VSize>() const
	{
		return *this;
	}

	template<TTNumbrEqual(Size, 3)>
	DPoint(T x=0, T y=0, T z=0) :
		DVector(x, y, z, 1.0)
	{}

	template<TTNumbrEqual(Size, 3)>
	explicit DPoint(const DVector<T, VSize>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, 1);
	}

	template<TTNumbrEqual(Size, 3)>
	explicit DPoint(DVector<T, 3>&& v) :
		xmm(std::move(v.xmm))
	{
		insert(3, 1);
	}


	template<TTNumbrEqual(Size, 2)>
	DPoint(T x = 0, T y = 0) :
		DVector(x, y, 1.0)
	{
	}

	template<TTNumbrEqual(Size, 2)>
	explicit DPoint(const DVector<T, VSize>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		insert(2, 1);
		insert(3, 0);
	}

	template<TTNumbrEqual(Size, 2)>
	explicit DPoint(DVector<T, 2>&& v) :
		xmm(std::move(v.xmm))
	{
		insert(2, 1);
		insert(3, 0);
	}
};

using DPoint3f = typename DPoint<float, 3>;
using DPoint3d = typename DPoint<double, 3>;
using DPoint2f = typename DPoint<float, 3>;
using DPoint2d = typename DPoint<double, 3>;

WML_END

