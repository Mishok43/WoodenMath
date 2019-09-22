#pragma once
#include "stdafx.h"
#include <limits>
#include "DVector.h"
#include "DPoint.h"
#include "DRay.h"

WML_BEGIN
template<typename T, uint8_t VSize, uint8_t alignment = sse_alignment_size_v<__m_t<T>>>
class alignas(alignment) DBounds
{
public:
	using PointT = typename DPoint<T, VSize>;

	PointT pMin, pMax;

	DBounds() :
		pMax(PointT(std::numeric_limits<T>::lowest())),
		pMin(PointT(std::numeric_limits<T>::infinity()))
	{
	}

	explicit DBounds(const PointT& p) :
		pMin(p), pMax(p)
	{
	}

	explicit DBounds(const DBounds& b, const PointT& p) :
		pMin(minv(b.pMin, p)),
		pMax(maxv(b.pMax, p))
	{}

	explicit DBounds(const DBounds& b0, const DBounds& b1) :
		pMin(minv(b0.pMin, b1.pMin)),
		pMax(maxv(b0.pMax, b1.pMax))
	{}

	DBounds(const PointT& p1, const PointT& p2):
		pMin(minv(p1, p2)),
		pMax(maxv(p1, p2))
	{}

	DBounds(PointT pMin, PointT pMax, std::true_type&& p1IsMinp2IsMax) :
		pMin(std::move(pMin)),
		pMax(std::move(pMax))
	{}

	inline const PointT& operator[](uint8_t i) const{ return *(&pMin+i); }
	inline PointT& operator[](uint8_t i){ return *(&pMin+i); };

	bool intersect(const DRayf& ray, const float tNear, const float tFar) const
	{
		tNear = 0;
		tFar = ray.tMax;
		for (uint8_t i = 0; i < 3; i++)
		{
			float invRayDir = ray.dir[i];
			float tNearCur = invRayDir * (pMin[i] - ray.origin[i]);
			float tFarCur = invRayDir * (pMax[i] - ray.origin[i]);
			if (tNearCur > tFarCur)
			{
				std::swap(tNearCur, tFarCur);
			}

			if (tNearCur > tNear)
			{
				tNear = tNearCur;
			}

			if (tFarCur < tFar)
			{
				tFar = tFarCur;
			}

			if (tNear > tFar)
			{
				return false;
			}
		}

		return true;
	}

	bool intersect(const DRayf& ray, const DVector3f& invDir, const int dirIsNeg[3]) const
	{
		const DBounds& bounds = *this;

		float tMin = (bounds[dirIsNeg[0]].x() - ray.origin.x())*invDir.x();
		float tMax = (bounds[1 - dirIsNeg[0]].x() - ray.origin.x())*invDir.x();

		float tYMin = (bounds[dirIsNeg[1]].y() - ray.origin.y())*invDir.y();
		float tYMax = (bounds[1 - dirIsNeg[1]].y()- ray.origin.y())*invDir.y();

		if (tMin > tYMax || tYMin > tMax) return false;
		if (tYMin > tMin) tMin = tYMin;
		if (tYMax < tMax) tMax = tYMax;

		float tZMin = (bounds[dirIsNeg[1]].z() - ray.origin.z())*invDir.z();
		float tZMax = (bounds[1 - dirIsNeg[1]].z() - ray.origin.z())*invDir.z();

		if (tMin > tZMax || tZMin > tMax) return false;
		if (tZMin > tMin) tMin = tZMin;
		if (tZMax < tMax) tMax = tZMax;

		return (tMin < ray.tMax) && (tMax > 0);
	}

	PointT corner(uint8_t i = 0) const
	{
		return PointT((*this)[(i & 1)].x(),
						   (*this)[(i & 2) >> 1].y(),
						   (*this)[(i & 4) >> 2].z());
	}

	bool inside(const PointT& p) const 
	{
		return p.x() >= pMin.x() && p.x() <= pMax.x() &&
			p.y() >= pMin.y() && p.y() <= pMax.y() &&
			p.z() >= pMin.z() && p.z() <= pMax.z();
	}

	bool insideExclusive(const PointT& p) const
	{
		return p.x() > pMin.x() && p.x() < pMax.x() &&
			p.y() > pMin.y() && p.y() < pMax.y() &&
			p.z() > pMin.z() && p.z() < pMax.z();
	}

	inline void expand(T delta)
	{
		expand(PointT(delta));
	}

	inline void expand(const PointT& delta)
	{
		pMin -= delta;
		pMax += delta;
	}

	inline PointT diagonal() const
	{
		return pMax - pMin;
	}

	inline T area() const
	{
		PointT d = diagonal();
		if constexpr (VSize == 3)
		{
			// x*y + y*z+z*x
			PointT d2 = d.permute<0b11001001>(); // y z x w
			return 2*PointT::dot<3>(d, d2);
		}
		else
		{
			return d.x()*d.y();
		}
	}


	inline T volume() const
	{
		static_assert(VSize == 3);

		PointT d = diagonal();
		return d.x()*d.y()*d.z();
	}

	uint8_t maxExtent() const
	{
		PointT d = diagonal();
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

	inline PointT lerp(const PointT& t) const
	{
		return PointT.lerp(pMin, pMax, t);
	}

	inline PointT getLerpFactors(const PointT& v) const
	{
		PointT o = v - pMin;
		return o / diagonal();
	}

	inline void boundingSphere(PointT& center, T& radius) const
	{
		center = (pMin + pMax) / 2;
		radius = (pMax - center).length();
	}

public:
	template<typename lT, uint8_t lVSize>
	static DBounds<lT, lVSize> intersect(const DBounds<lT, lVSize>& b1, const DBounds<lT, lVSize>& b2)
	{
		return DBounds<lT, lVSize>(
			maxv(b1.pMin, b2.pMin),
			minv(b1.pMax, b2.pMax),
			std::true_type());
	}

	template<typename lT, uint8_t lVSize>
	static bool overlaps(const DBounds<lT, lVSize>& b1, const DBounds<lT, lVSize>& b2)
	{
		bool x = (b1.pMax.x() >= b2.pMin.x()) && (b1.pMin.x() <= b2.pMax.x());
		bool y = (b1.pMax.y() >= b2.pMin.y()) && (b1.pMin.y() <= b2.pMax.y());
		bool z = (b1.pMax.z() >= b2.pMin.z()) && (b1.pMin.z() <= b2.pMax.z());
		return x && y && z;
	}
};


template<typename lT, typename Func>
void for_each(const DBounds<lT, 2>& bounds, Func&& f)
{
	DPoint<lT, 2> diagonal = bounds.diagonal();
	for (uint32_t i = 0; i <= diagonal.x(); i++)
	{
		for (uint32_t j = 0; j <= diagonal.y(); j++)
		{
			f(bounds.pMin + DPoint<lT, 2>(i, j), DPoint<lT, 2>(i, j), i*(diagonal.x()+1) + j);
		}
	}
}

using DBounds2u = typename DBounds<uint32_t, 2>;
using DBounds2i = typename DBounds<int32_t, 2>;
using DBounds2f = typename DBounds<float, 2>;
using DBounds2d = typename DBounds<double, 2>;

using DBounds3u = typename DBounds<uint32_t, 3>;
using DBounds3i = typename DBounds<int32_t, 3>;
using DBounds3f = typename DBounds<float, 3>;
using DBounds3d = typename DBounds<double, 3>;
WML_END

