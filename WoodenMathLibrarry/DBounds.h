#pragma once
#include "stdafx.h"
#include "DVector.h"

WML_BEGIN
template<typename T, uint8_t VSize, uint8_t alignment = wml::sse_alignment_size_v<wml::__m_t<T>>>
class alignas(alignment) DBounds
{
public:
	using vector_type = typename DVector<T, VSize>;

	vector_type pMin, pMax;

	DBounds():
		pMax(std::numeric_limits<T>::lowest()),
		pMin(std::numeric_limits<T>::max())
	{}

	explicit DBounds(const vector_type& p):
		pMin(p), pMax(p)
	{ }

	DBounds(const vector_type& p1, const vector_type& p2):
		pMin(vector_type::minVector(p1, p2)),
		pMax(vector_type::maxVector(p1, p2))
	{}

	DBounds(vector_type pMin, vector_type pMax, std::true_type&& p1IsMinp2IsMax) :
		pMin(std::move(p1)),
		pMax(std::move(p2))
	{}

};


using DBounds2u = typename DBounds<uint32_t, 2>;
using DBounds2i = typename DBounds<int32_t, 2>;
using DBounds2f = typename DBounds<float, 2>;
using DBounds2d = typename DBounds<double, 2>;

using DBounds3u = typename DBounds<uint32_t, 3>;
using DBounds3i = typename DBounds<int32_t, 3>;
using DBounds3f = typename DBounds<float, 3>;
using DBounds3d = typename DBounds<double, 3>;
WML_END

