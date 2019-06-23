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

	DBounds() :
		pMax(std::numeric_limits<T>::lowest()),
		pMin(std::numeric_limits<T>::max())
	{
	}

	explicit DBounds(const vector_type& p) :
		pMin(p), pMax(p)
	{
	}

	explicit DBounds(const DBounds& b, const vector_type& p) :
		pMin(vector_type::minVector(b.pMin, p)),
		pMax(vector_type::maxVector(b.pMax, p))
	{}

	explicit DBounds(const DBounds& b0, const DBounds& b1) :
		pMin(vector_type::minVector(b0.pMin, b1.pMin)),
		pMax(vector_type::maxVector(b0.pMax, b1.pMax)),
	{}

	DBounds(const vector_type& p1, const vector_type& p2):
		pMin(vector_type::minVector(p1, p2)),
		pMax(vector_type::maxVector(p1, p2))
	{}

	DBounds(vector_type pMin, vector_type pMax, std::true_type&& p1IsMinp2IsMax) :
		pMin(std::move(p1)),
		pMax(std::move(p2))
	{}

	inline const vector_type& operator[](uint8_t i = 0) const{ return *(&pMin+i); }
	inline vector_type& operator[](uint8_t i = 0){ return *(&pMin+i); };

	vector_type corner(uint8_t i = 0) const
	{
		return vector_type((*this)[(i & 1)].x(),
						   (*this)[(i & 2) >> 1].y(),
						   (*this)[(i & 4) >> 2].z());
	}

	bool inside(const vector_type& p) const 
	{
		return p.x() >= pMin.x() && p.x() <= pMax.x() &&
			p.y() >= pMin.y() && p.y() <= pMax.y() &&
			p.z() >= pMin.z() && p.z() <= pMax.z();
	}

	bool insideExclusive(const vector_type& p) const
	{
		return p.x() > pMin.x() && p.x() < pMax.x() &&
			p.y() > pMin.y() && p.y() < pMax.y() &&
			p.z() > pMin.z() && p.z() < pMax.z();
	}

	inline void expand(T delta)
	{
		expand(vector_type(delta));
	}

	inline void expand(const vector_type& delta)
	{
		pMin -= delta;
		pMax += delta;
	}

	inline vector_type diagonal() const
	{
		return pMax - pMin;
	}

	inline T surfaceArea() const
	{
		vector_type d = diagonal();
		if constexpr (VSize == 3)
		{
			// x*y + y*z+z*x
			vector_type d2 = d.permute<0b11001001>(); // y z x w
			return 2*vector_type::dot<3>(d, d2);
		}
		else
		{
			return d.x()*d.y();
		}
	}


	inline T volume() const
	{
		static_assert(VSize == 3);

		vector_type d = diagonal();
		return d.x()*d.y()*d.z();
	}

	uint8_t maxExtent() const
	{
		vector_type d = diagonal();
		if constexpr (VSize == 3)
		{
			if (d.x() > d.y() && d.x() > d.z())
			{
				return 0;
			}
			else if (d.y() > d.z())
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			return (d.x() > d.y()) ? 0 : 1;
		}
	}

	inline vector_type lerp(const vector_type& t) const
	{
		return vector_type.lerp(pMin, pMax, t);
	}

	inline vector_type getLerpFactors(const vector_type& v) const
	{
		vector_type o = v - pMin;
		return o / diagonal();
	}

	inline boundingSphere(vector_type& center, T& radius) const
	{
		center = (pMin + pMax) / 2;
		radius = (pMax - center).length();
	}

public:
	template<typename lT, uint8_t lVSize>
	static DBounds<lT, lVSize> intersect(const DBounds<lT, lVSize>& b1, const DBounds<lT, lVSize>& b2)
	{
		using lvector_type = typename DVector<lT, lVSize>;
		return DBounds<lT, lVSize>(
			lvector_type::maxVector(b1.pMin, b2.pMin),
			lvector_type::minVector(b1.pMax, b2.pMax),
			std::true_type());
	}

	template<typename lT, uint8_t lVSize>
	static bool overlaps(const DBounds<lT, lVSize>& b1, const DBounds<lT, lVSize>& b2)
	{
		using lvector_type = typename DVector<lT, lVSize>;
		bool x = (b1.pMax.x() >= b2.pMin.x()) && (b1.pMin.x() <= b2.pMax.x());
		bool y = (b1.pMax.y() >= b2.pMin.y()) && (b1.pMin.y() <= b2.pMax.y());
		bool z = (b1.pMax.z() >= b2.pMin.z()) && (b1.pMin.z() <= b2.pMax.z());
		return x && y && z;
	}
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

