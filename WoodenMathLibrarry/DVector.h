#pragma once
#include <cmath>
#include <math.h>
#include "TypedSSE.h"

// TODO: fork some methods for different sizes 

using namespace TypedSSE;

using DVector2f = DVector<float, 2>;
using DVector3f = DVector<float, 3>;
using DVector4f = DVector<float, 4>;

using DVector2d = DVector<double, 2>;
using DVector3d = DVector<double, 3>;
using DVector4d = DVector<double, 4>;

using DVector2i = DVector<int32_t, 2>;
using DVector3i = DVector<int32_t, 3>;
using DVector4i = DVector<int32_t, 4>;

using DVector2u = DVector<uint32_t, 2>;
using DVector3u = DVector<uint32_t, 3>;
using DVector4u = DVector<uint32_t, 4>;

template<typename T, uint8_t size>
class DVector
{
public:
	DVector(T x, T y, T z, T w):
		x(x), y(y), z(z), w(w)
	{ 
		constructorAsserts();
	}

	DVector() :
		x(0), y(0), z(0), w(0)
	{
		constructorAsserts();
	}

	DVector(__m_t<T> data) 
	{
		constructorAsserts();
		_mm_store_t(&x, data);
	}

	inline void constructorAsserts() const
	{
		static_assert(std::is_same<float, T>() ||
					  std::is_same<double, T>() ||
					  std::is_same<int32_t, T>() ||
					  std::is_same<uint32_t, T>(),
					  "float, double, uint32_t, int32_t are only supported types!");
	}

	T x, y, z, w;


	DVector operator+(const DVector& v) const
	{

		__m_t<T> v0 =  _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v.x);
		__m_t<T> vres = _mm_add_t<T>(v0, v1);

		return DVector<T, size>(vres);
	}

	DVector & operator+=(const DVector& v)
	{
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v.x);
		__m_t<T> vres = _mm_add_t<T>(v0, v1);

		_mm_store_t(&x, vres);
		
		return *this;
	}

	DVector operator-(const DVector& v) const
	{

		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v.x);
		__m_t<T> vres = _mm_sub_t<T>(v0, v1);

		return DVector<T, size>(vres);
	}

	DVector& operator-=(const DVector& v)
	{
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v.x);
		__m_t<T> vres = _mm_sub_t<T>(v0, v1);

		_mm_store_t(&x, vres);

		return *this;
	}

	DVector operator*(float s) const
	{
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_set1_t(s);
		__m_t<T> vres = _mm_mul_t<T>(v0, v1);

		return DVector<T, size>(vres);
	}

	DVector& operator*=(T s)
	{
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_set1_t(s);

		__m_t<T> vres = _mm_mul_t<T>(v0, v1);

		_mm_store_t(&x, vres);

		return *this;
	}

	DVector operator/(T s) const
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		assert(s != 0);
		T inv = 1.0 / s;

		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_set1_t(inv);

		__m_t<T> vres = _mm_mul_t<T>(v0, v1);

		return DVector<T, size>(vres);
	}

	DVector& operator/=(T s)
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		assert(s != 0);
		T inv = 1.0 / s;

		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_set1_t(inv);

		__m_t<T> vres = _mm_mul_t<T>(v0, v1);

		_mm_store_t(&x, vres);
	}

	DVector operator-() const
	{
		return DVector<T, size>(-x, -y, -z, -w);
	}

	T& operator[](uint32_t i)
	{
		assert(i >= 0 && i < size);
		return (&x)[i];
	}

	T operator[](uint32_t i) const
	{
		assert(i >= 0 && i < size);
		return (&x)[i];
	}

	bool HasOnlyNaNs() const
	{
		return std::isnan(x) && std::isnan(y) && std::isnan(z) && std::isnan(w);
	}

	float Length2() const
	{
		return Dot(*this, *this);
	}

	float Length() const
	{
		return std::sqrt(Dot(*this, *this));
	}

public:
	static inline DVector Abs(const DVector& v)
	{
		return DVector(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
	}

	static inline T Dot(const DVector& v1, const DVector& v2)
	{
		__m_t<T> v1data = _mm_load_t(&v1.x);
		__m_t<T> v2data = _mm_load_t(&v2.x);
		__m_t<T> vres = _mm_mul_t<T>(v1data, v2data);

		T res = 0;
		for (uint8_t i = 0; i < size; ++i)
		{
			res += ((T*)(&vres))[i];
		}
	}

	static inline T AbsDot(const DVector& v1, const DVector& v2)
	{
		return std::abs(Dot(v1, v2));
	}

	template<typename sizeCheck = std::enable_if_t<size == 3>>
	static inline DVector Cross(const DVector& v1, const DVector& v2)
	{
		__m_t<T> v1 = _mm_load_t(&v1.x);
		__m_t<T> v2 = _mm_load_t(&v2.x);
		v1 = _mm_permute_t(v1, 0b11001001); // Y Z X W
		v2 = _mm_permute_t(v2, 0b11010010); // Z X Y W

		__m_t<T> v3 = _mm_mul_t(v1, v3);
		v1 = _mm_permute_t(v1, 0b11001001); // Z X Y W
		v2 = _mm_permute_t(v2, 0b11010010); // Y Z X W
		__m_t<T> v4 = _mm_mul_t(v1, v2);
		__m_t<T> vres = _mm_sub_t(v3, v4);
		return DVector(vres);

		/*
		double v1x = v1.x, v1y = v1.y, v1z = v1.z;
		double v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return DVector(
			(v1y*v2z) - (v1z*v2y),
			(v1z*v2x) - (v1x*v2z),
			(v1x*v2y) - (v1y*v2x));*/
	}


	static inline DVector Normalize(const DVector& v)
	{
		return v / v.Length();
	}

	static inline T MinComponent(const DVector& v)
	{
		return std::min<T>(v.x, std::min<T>(v.y, v.z));
	}

	static inline T MaxComponent(const DVector& v)
	{
		return std::max<T>(v.x, std::max<T>(v.y, v.z));
	}

	static inline DVector Min(const DVector& v1, const DVector& v2)
	{

		return DVector<T, size>(std::min<T>(v1.x, v2.x),
						   std::min<T>(v1.y, v2.y),
						   std::min<T>(v1.z, v2.z),
						   std::min<T>(v1.w, v2.w));
	}

	static inline DVector Max(const DVector& v1, const DVector& v2)
	{
		return DVector<T, size>(std::max<T>(v1.x, v2.x),
						   std::max<T>(v1.y, v2.y),
						   std::max<T>(v1.z, v2.z),
						std::max<T>(v1.w, v2.w));
	}

	/*static inline void MakeBasisByVector(const DVector& v1, DVector* v2, DVector* v3)
	{
		if (std::abs(v1.x) > std::abs(v1.y))
		{
			*v2 = DVector(-v1.z, 0, v1.x)
		}
	}*/
};
