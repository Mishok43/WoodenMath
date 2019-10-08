#pragma once

#include "stdafx.h"
#include "DVector.h"

WML_BEGIN

using namespace TypedSSE;

template<typename T, uint8_t Size>
class DNormal: public DVector<T, Size>
{
public:
	using base = typename DVector<T, Size>;


	template<TTNumbrEqual(Size, 3)>
	DNormal(T x, T y, T z):
		base(x, y, z)
	{
	}
	DNormal(T broadcastValue = 0.0f) :
		base(broadcastValue)
	{
	//	this->insert(3, (T)0);

	}


	template<TTNumbrEqual(Size, 3)>
	DNormal(const DVector<T, Size>& v):
		base(v)
	{
	//	this->insert(3, 0);
	}

	template<TTNumbrEqual(Size, 3)>
	DNormal(DVector<T, 3>&& v) :
		base(v)
	{
	//	this->insert(3, 0);
	}

	operator DVector<T, Size>() const
	{
		return *this;
	}	

	template<TTNumbrEqual(Size, 2)>
	DNormal(T x, T y) :
		base(x, y)
	{}
	
	template<TTNumbrEqual(Size, 2)>
	DNormal(const DVector<T, Size>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		this->insert(3, 0);
	//	this->insert(2, 0);
	}

	template<TTNumbrEqual(Size, 2)>
	DNormal(DVector<T, 2>&& v) :
		base(std::move(v.xmm))
	{
		this->insert(3, 0);
	//	this->insert(2, 0);
	}

};

template<typename VT, uint8_t VSize>
inline DNormal<VT, VSize> faceForward(const DNormal<VT, VSize>& n, const DVector<VT, VSize>& v0)
{
	if (dot(n, v0) < 0.0)
	{
		return -n;
	}
	else
	{
		return n;
	}
}

using DNormal3f = typename DNormal<float, 3>;
using DNormal3d = typename DNormal<double, 3>;
using DNormal2f = typename DNormal<float, 3>;
using DNormal2d = typename DNormal<double, 3>;

WML_END

