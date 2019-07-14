#pragma once

#include "stdafx.h"
#include "DVector.h"

WML_BEGIN

using namespace TypedSSE;

template<typename T, uint8_t Size>
class DNormal: public DVector<T, Size>
{
public:

	template<TTNumbrEqual(Size, 3)>
	DNormal(T x=0, T y=0, T z=0):
		DVector(x, y, z)
	{
	}
	

	template<TTNumbrEqual(Size, 3)>
	explicit DNormal(const DVector<T, Size>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, 0);
	}

	template<TTNumbrEqual(Size, 3)>
	explicit DNormal(DVector<T, 3>&& v) :
		xmm(std::move(v.xmm))
	{
		insert(3, 0);
	}

	operator DVector<T, Size>() const
	{
		return *this;
	}	

	template<TTNumbrEqual(Size, 2)>
	DNormal(T x=0, T y=0) :
		DVector(x, y)
	{}

	template<TTNumbrEqual(Size, 2)>
	explicit DNormal(const DVector<T, Size>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, 0);
		insert(2, 0);
	}

	template<TTNumbrEqual(Size, 2)>
	explicit DNormal(DVector<T, 2>&& v) :
		xmm(std::move(v.xmm))
	{
		insert(3, 0);
		insert(2, 0);
	}

};

template<typename VT, uint8_t VSize>
inline DNormal<VT, VSize> faceForward(const DNormal<VT, VSize>& n, const DVector<VT, VSize>& v0)
{
	return  (dot(n, v0) < 0.0) ? -n : n;
}

using DNormal3f = typename DNormal<float, 3>;
using DNormal3d = typename DNormal<double, 3>;
using DNormal2f = typename DNormal<float, 3>;
using DNormal2d = typename DNormal<double, 3>;

WML_END

