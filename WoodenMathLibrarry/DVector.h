#pragma once
#include <cmath>
#include <cstring>
#include <math.h>

#include "TypedSSE.h"


using namespace TypedSSE;


template<typename T>
class DVector
{
public:
	DVector() :
		x(0), y(0), z(0), w(0)
	{
		static_assert(std::is_same<float, T>() || 
					  std::is_same<double, T>() || 
					  std::is_same<std::int32_t, T>() || 
					  std::is_same<std::uint32_t, T>(),
					  "float, double, uint32_t, int32_t are only supported types!");
	}

	DVector(const void* rdata) 
	{

		static_assert(std::is_same<float, T>() ||
					  std::is_same<double, T>() ||
					  std::is_same<std::int32_t, T>() ||
					  std::is_same<std::uint32_t, T>(),
					  "float, double, uint32_t, int32_t are only supported types!");

		std::memcpy(&x, rdata, sizeof(T) * 4);
	}

	T x, y, z, w;

	DVector operator+(const DVector& v) const
	{
		_mm_load_t(&x);



		return DVector(vres);
	}

	DVector & operator+=(const DVector& v)
	{
		__m128 v0 = _mm_load_ps(&x);
		__m128 v1 = _mm_load_ps(&v.x);
		__m128 vres = _mm_add_ps(v0, v1);

		//copyFromM128(vres);
		return *this;
	}

	DVector operator-(const DVector& v) const
	{
		__m128 v0 = _mm_load_ps(&x);
		__m128 v1 = _mm_load_ps(&v.x);

		__m128 vres = _mm_sub_ps(v0, v1);

		return DVector(vres);
	}

	DVector& operator-=(const DVector& v)
	{
		__m128 v0 = _mm_load_ps(&x);
		__m128 v1 = _mm_load_ps(&v.x);
		__m128 vres = _mm_sub_ps(v0, v1);

		copyFromM128(vres);
		return *this;
	}

	DVector operator*(float s) const
	{
		__m128 v0 = _mm_load_ps(&x);
		__m128 v1 = _mm_load_ps(&v.x);
		__m128 vres = _mm_sub_ps(v0, v1);
		return *this;
	}

	DVector& operator*=(T s)
	{
		x *= s; y *= s; z *= s;
		return *this;
	}

	DVector operator/(T s) const
	{
		assert(s != 0);
		float inv = 1.0f / s;

		return DVector(v.x*inv, v.y*inv, v.z*inv);
	}

	DVector& operator/=(T s)
	{
		assert(s != 0);
		float inv = 1.0f / s;

		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	DVector operator-() const
	{
		return DVector(-x, -y, -z);
	}

	T& operator[](uint32_t i)
	{
		assert(i >= 0 && i < 3);
		if (i == 0)
		{
			return x;
		}
		else
		{
			if (i == 1)
			{
				return y;
			}
			else
			{
				return z;
			}
		}
	}

	T operator[](uint32_t i) const
	{
		assert(i >= 0 && i < 3);
		if (i == 0)
		{
			return x;
		}
		else
		{
			if (i == 1)
			{
				return y;
			}
			else
			{
				return z;
			}
		}
	}

	bool HasOnlyNaNs() const
	{
		return std::isnan(x) && std::isnan(y) && std::isnan(z);
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
		return DVector(std::abs(v.x), std::abs(v.y), std:; abs(v.z));
	}

	static inline T Dot(const DVector& v1, const DVector& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	static inline T AbsDot(const DVector& v1, const DVector& v2)
	{
		return std::abs(Dot(v1, v2));
	}

	static inline DVector Cross(const DVector& v1, const DVector& v2)
	{
		double v1x = v1.x, v1y = v1.y, v1z = v1.z;
		double v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return DVector(
			(v1y*v2z) - (v1z*v2y),
			(v1z*v2x) - (v1x*v2z),
			(v1x*v2y) - (v1y*v2x));
	}

	static inline DVector Normalize(const DVector& v)
	{
		return v / v.Length();
	}

	static inline T MinComponent(const DVector& v)
	{
		return std::min(v.x, std::min(v.y, v.z));
	}

	static inline T MaxComponent(const DVector& v)
	{
		return std::max(v.x, std::max(v.y, v.z));
	}

	static inline DVector Min(const DVector& v1, const DVector& v2)
	{
		return DVector(std::min(v1.x, v2.x),
						   std::min(v1.y, v2.y),
						   std::min(v1.z, v2.z));
	}

	static inline DVector Max(const DVector& v1, const DVector& v2)
	{
		return DVector(std::max(v1.x, v2.x),
						   std::max(v1.y, v2.y),
						   std::max(v1.z, v2.z));
	}

	static inline void MakeBasisByVector(const DVector& v1, DVector* v2, DVector* v3)
	{
		if (std::abs(v1.x) > std::abs(v1.y))
		{
			*v2 = DVector(-v1.z, 0, v1.x)
		}
	}
};
/*
template<typename T>
class DVector2
{
public:
	DVector2() :
		x(0), y(0)
	{
	}

	DVector2(T x, T y) :
		x(x), y(y)
	{}

	
	float x, y, z, w; // Z and W not used!

	DVector2<T> operator+(const DVector2<T>& v) const
	{
		__m128 v0 = _mm_load_ps(&x);
		__m128 v1 = _mm_load_ps(&v.x);

		_mm_add_ps(v0, v1);
		
		return DVector2<T>(v.x, v.y);
	}

	DVector2<T>& operator+=(const DVector2<T>& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	DVector2<T> operator-(const DVector2<T>& v) const
	{
		return DVector2(x - v.x, y - v.y);
	}

	DVector2<T>& operator-=(const DVector2<T>& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	DVector2<T> operator*(T s) const
	{
		return DVector2(s*v.x, s*v.y);
	}

	DVector2<T>& operator*=(T s)
	{
		x *= s; y *= s;
		return *this;
	}

	DVector2<T> operator/(T s) const
	{
		assert(s != 0);
		float inv = 1.0f / s;

		return DVector2(v.x*inv, v.y*inv);
	}

	DVector2<T>& operator/=(T s)
	{
		assert(s != 0);
		float inv = 1.0f / s;

		x *= inv; y *= inv;
		return *this;
	}

	DVector2<T> operator-() const
	{
		return DVector2<T>(-x, -y);
	}

	T& operator[](uint32_t i)
	{
		assert(i >= 0 && i < 2);
		if (i == 0)
		{
			return x;
		}
		else
		{
			return y;
		}
	}

	T operator[](uint32_t i) const
	{
		assert(i >= 0 && i < 2);
		if (i == 0)
		{
			return x;
		}
		else
		{
			return y;
		}
	}

	bool HasOnlyNaNs() const
	{
		return std::isnan(x) && std::isnan(y);
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
	static inline DVector2<T> Abs(const DVector2<T>& v)
	{
		return DVector2<T>(std::abs(v.x), std::abs(v.y));
	}

	static inline T Dot(const DVector2<T>& v1, const DVector2<T>& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	static inline T AbsDot(const DVector2<T>& v1, const DVector2<T>& v2)
	{
		return std::abs(Dot(v1, v2));
	}

	static inline DVector2<T> Normalize(const DVector2<T>& v)
	{
		return v / v.Length();
	}

	static T MinComponent(const DVector2<T>& v)
	{
		return std::min(v.x, v.y);
	}

	static T MaxComponent(const DVector2<T>& v)
	{
		return std::max(v.x, v.y);
	}

	static DVector2<T> Min(const DVector2<T>& v1, const DVector2<T>& v2)
	{
		return DVector2<T>(std::min(v1.x, v2.x),
						  std::min(v1.y, v2.y));
	}

	static DVector2<T> Max(const DVector2<T>& v1, const DVector2<T>& v2)
	{
		return DVector2<T>(std::max(v1.x, v2.x),
						  std::max(v1.y, v2.y));
	}
};

using DVectorf = DVector<float>;
using DVectord = DVector<double>;
using DVectori = DVector<int>;

template<typename T>
class DVector
{
public:
	DVector() :
		x(0), y(0), z(0)
	{
	}

	Vector2(T x, T y, T z) :
		x(x), y(y), z(z)
	{}

	T x, y, z;


	DVector operator+(const DVector& v) const
	{
		return Vector2(x + v.x, y + v.y, z + v.z);
	}

	DVector& operator+=(const DVector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	DVector operator-(const DVector& v) const
	{
		return Vector2(x - v.x, y - v.y, z - v.z);
	}

	DVector& operator-=(const DVector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	DVector operator*(T s) const
	{
		return DVector(s*v.x, s*v.y, s*v.z);
	}

	DVector& operator*=(T s)
	{
		x *= s; y *= s; z *= s;
		return *this;
	}

	DVector operator/(T s) const
	{
		assert(s != 0);
		float inv = 1.0f / s;

		return DVector(v.x*inv, v.y*inv, v.z*inv);
	}

	DVector& operator/=(T s)
	{
		assert(s != 0);
		float inv = 1.0f / s;

		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	DVector operator-() const
	{
		return DVector(-x, -y, -z);
	}

	T& operator[](uint32_t i)
	{
		assert(i >= 0 && i < 3);
		if (i == 0)
		{
			return x;
		}
		else
		{
			if (i == 1)
			{
				return y;
			}
			else
			{
				return z;
			}
		}
	}

	T operator[](uint32_t i) const
	{
		assert(i >= 0 && i < 3);
		if (i == 0)
		{
			return x;
		}
		else
		{
			if (i == 1)
			{
				return y;
			}
			else
			{
				return z;
			}
		}
	}

	bool HasOnlyNaNs() const
	{
		return std::isnan(x) && std::isnan(y) && std::isnan(z);
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
		return DVector(std::abs(v.x), std::abs(v.y), std:; abs(v.z));
	}

	static inline T Dot(const DVector& v1, const DVector& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	static inline T AbsDot(const DVector& v1, const DVector& v2)
	{
		return std::abs(Dot(v1, v2));
	}

	static inline DVector Cross(const DVector& v1, const DVector& v2)
	{
		double v1x = v1.x, v1y = v1.y, v1z = v1.z;
		double v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return DVector(
			(v1y*v2z) - (v1z*v2y),
			(v1z*v2x) - (v1x*v2z),
			(v1x*v2y) - (v1y*v2x));
	}

	static inline DVector Normalize(const DVector& v)
	{
		return v / v.Length();
	}

	static inline T MinComponent(const DVector& v)
	{
		return std::min(v.x, std::min(v.y, v.z));
	}

	static inline T MaxComponent(const DVector& v)
	{
		return std::max(v.x, std::max(v.y, v.z));
	}

	static inline DVector Min(const DVector& v1, const DVector& v2)
	{
		return DVector(std::min(v1.x, v2.x),
						  std::min(v1.y, v2.y),
						  std::min(v1.z, v2.z));
	}

	static inline DVector Max(const DVector& v1, const DVector& v2)
	{
		return DVector(std::max(v1.x, v2.x),
						  std::max(v1.y, v2.y),
						  std::max(v1.z, v2.z));
	}

	static inline void MakeBasisByVector(const DVector& v1, DVector* v2, DVector* v3)
	{
		if (std::abs(v1.x) > std::abs(v1.y))
		{
			*v2 = DVector(-v1.z, 0, v1.x)
		}
	}
};*/