#pragma once
#include "stdafx.h"
#include "TypedSSE.h"

WML_BEGIN

// TODO: fork some methods for different sizes 

using namespace TypedSSE;

template<typename T, uint8_t size>
class alignas(16) DVector 
{
public:
	DVector(T x, T y=0, T z=0, T w=0):
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

	DVector operator*(T s) const
	{
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_set1_t(s);
		__m_t<T> vres = _mm_mul_t<T>(v0, v1);

		return DVector<T, size>(vres);
	}

	DVector operator*(DVector v) const
	{
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v.x);
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

	DVector& operator*=(DVector v)
	{
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v.x);
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
		return *this;

	}

	DVector operator/(DVector v2) const
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		//assert(v2.x != 0);

		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v2.x);

		__m_t<T> vres = _mm_div_t<T>(v0, v1);


		return DVector<T, size>(vres);
	}

	DVector& operator/=(DVector v2)
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		//assert(s != 0);
		__m_t<T> v0 = _mm_load_t(&x);
		__m_t<T> v1 = _mm_load_t(&v2.x);

		__m_t<T> vres = _mm_div_t<T>(v0, v1);

		_mm_store_t(&x, vres);
		return *this;
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

	bool hasOnlyNaNs() const
	{
		return std::isnan(x) && std::isnan(y) && std::isnan(z) && std::isnan(w);
	}

	T length2() const
	{
		return dot(*this, *this);
	}

	T length() const
	{
		return std::sqrt(dot(*this, *this));
	}

public:
	static inline DVector abs(const DVector& v)
	{
		return DVector(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
	}

	static inline T dot(const DVector& v1, const DVector& v2)
	{
		__m_t<T> v1data = _mm_load_t(&v1.x);
		__m_t<T> v2data = _mm_load_t(&v2.x);
		__m_t<T> vres = _mm_mul_t<T>(v1data, v2data);

		T res = 0;
		for (uint8_t i = 0; i < size; ++i)
		{
			res += ((T*)(&vres))[i];
		}
		return res;
	}

	static inline T absDot(const DVector& v1, const DVector& v2)
	{
		return std::abs(dot(v1, v2));
	}

	template<typename sizeCheck = std::enable_if_t<size == 3>>
	static inline DVector cross(const DVector& v1, const DVector& v2)
	{
		__m_t<T> v1data = _mm_load_t(&v1.x);
		__m_t<T> v2data = _mm_load_t(&v2.x);

		v1data = _mm_permute_ts<0b11001001, T>::f(v1data); // Y Z X W
		v2data  = _mm_permute_ts<0b11010010, T>::f(v2data); // Z X Y W

		__m_t<T> v3data = _mm_mul_t<T>(v1data, v2data);
		v1data = _mm_permute_ts<0b11001001, T>::f(v1data); // Z X Y W
		v2data = _mm_permute_ts<0b11010010, T>::f(v2data); // Y Z X W

		__m_t<T> v4data = _mm_mul_t<T>(v1data, v2data);
		__m_t<T> vres = _mm_sub_t<T>(v3data, v4data);
		return DVector(vres);
	}


	static inline DVector normalize(const DVector& v)
	{
		return v / v.length();
	}

	static inline T minComponent(const DVector& v)
	{
		return std::min<T>(v.x, std::min<T>(v.y, v.z));
	}

	static inline T maxComponent(const DVector& v)
	{
		return std::max<T>(v.x, std::max<T>(v.y, v.z));
	}

	static inline DVector minVector(const DVector& v1, const DVector& v2)
	{

		return DVector<T, size>(std::min<T>(v1.x, v2.x),
						   std::min<T>(v1.y, v2.y),
						   std::min<T>(v1.z, v2.z),
						   std::min<T>(v1.w, v2.w));
	}

	static inline DVector maxVector(const DVector& v1, const DVector& v2)
	{
		return DVector<T, size>(std::max<T>(v1.x, v2.x),
						   std::max<T>(v1.y, v2.y),
						   std::max<T>(v1.z, v2.z),
						std::max<T>(v1.w, v2.w));
	}

	static inline void makeBasisByVector(const DVector& v1, DVector& v2, DVector& v3)
	{
		if (std::abs(v1.x) > std::abs(v1.y))
		{
			v2 = DVector(-v1.z, 0, v1.x);
		}
		else
		{
			v2 = DVector(0, v1.z, -v1.y);
		}

		v2 = v2.normalize(v2);;
		v3 = cross(v1, v2);
	}
};


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

WML_END