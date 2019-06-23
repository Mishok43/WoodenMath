#pragma once

#include "stdafx.h"
#include "DVector.h"

WML_BEGIN

using namespace TypedSSE;

template<typename T, typename __mT = __m_t<T>, uint8_t alignment = sse_alignment_size_v<__mT>>
class alignas(alignment) DNormal: public DVector<T, 3>
{
public:
	DNormal(T x, T y, T z):
		DVector(x, y, z)
	{}
	
	explicit DNormal(const DVector<T, 3>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, 0);
	}

	explicit DNormal(DVector<T, 3>&& v) :
		xmm(std::move(v.xmm))
	{
		insert(3, 0);
	}

	operator DVector<T, 3>() const
	{
		return *this;
	}
};

WML_END

