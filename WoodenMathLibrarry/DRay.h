#pragma once
#include "stdafx.h"
#include "DVector.h"

WML_BEGIN

template<typename T, typename __mT = __m_t<T>, uint8_t alignment = sse_alignment_size_v<__mT>>
class alignas(alignment) DRay
{
	using vector_type = typename DVector<T, 4>;

public:
	DRay()
	{}

	DRay(wml::DVector<T, 3> o, wml::DVector<T, 3> d):
		origin(std::move(o)),
		dir(std::move(d))
	{}

	inline DVector<T, 3> operator()(T t) const
	{
		return vector_type::mAdd(dir, t, origin);
	}

	wml::DVector<T, 3> origin;
	wml::DVector<T, 3> dir;
};


WML_END