#pragma once
#include "stdafx.h"
#include "TypedSSE.h"

WML_BEGIN

// TODO: fork some methods for different sizes 

using namespace TypedSSE;

template<typename T, uint8_t Size, typename __mT = __m_t<T>, uint8_t alignment = sse_alignment_size_v<__mT>>
class alignas(alignment) DVector
{
public:
	__mT xmm;

	DVector(T* data)
	{
		xmm = _mm_loadu_t(data);
	}
	
	DVector(T broadcastValue = 0)
	{
		xmm = _mm_set1_t<T, __mT>(broadcastValue);
	}

	template<typename check = std::enable_if_t<Size == 4>>
	DVector(T x, T y, T z, T w)
	{
		xmm = _mm_setr_t<T>(x, y, z, w);
	}

	template<typename check = std::enable_if_t<Size == 3>>
	DVector(T x, T y, T z)
	{
		xmm = _mm_setr_t<T>(x, y, z, 0);
	}

	template<typename check = std::enable_if_t<Size == 2>>
	DVector(T x, T y)
	{
		xmm = _mm_setr_t<T>(x, y, 0, 0);
	}


	DVector(const __m_t<T>& data) :
		xmm(data)
	{
	}

	DVector( __m_t<T>&& data) :
	   xmm(std::move(data))
	{}


	template<typename = std::enable_if_t<Size == 3>>
	explicit DVector(const DVector<T, 4>& v) 
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, 0);
	}

	template<typename = std::enable_if_t<Size == 4>>
	explicit DVector(const DVector<T, 3>& v, T w=0) 
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, w);
	}

	template<typename = std::enable_if_t<Size == 3>>
	explicit DVector(DVector<T, 4>&& v):
		xmm(std::move(v.xmm))
	{
		insert(3, 0);
	}

	template<typename = std::enable_if_t<Size == 4>>
	explicit DVector(DVector<T, 3>&& v, T w = 0):
		xmm(std::move(v.xmm))
	{
		insert(3, w);
	}

	template<typename check = std::enable_if_t<std::is_same_v<__mT, __m128>>>
	operator __m128() const
	{
		return xmm;
	}

	template<typename check = std::enable_if_t<std::is_same_v<__mT, __m128i>>>
	operator __m128i() const
	{
		return xmm;
	}

	template<typename check = std::enable_if_t<std::is_same_v<__mT, __m256d>>>
	operator __m256d() const
	{
		return xmm;
	}

	template<typename check = std::enable_if_t<std::is_same_v<T, float>>>
	float* data()
	{
		return xmm.m128_f32;
	}

	template<typename check = std::enable_if_t<std::is_same_v<T, double>>>
	double* data()
	{
		return xmm.m256d_f64;
	}

	template<typename check = std::enable_if_t<std::is_same_v<T, int32_t>>>
	int32_t* data()
	{
		return xmm.m128i_i32;
	}

	template<typename check = std::enable_if_t<std::is_same_v<T, uint32_t>>>
	uint32_t* data()
	{
		return xmm.m128i_u32;
	}

	template<typename check = std::enable_if_t<std::is_same_v<float, T>>>
	const float* data() const
	{
		return xmm.m128_f32;
	}

	template<typename check = std::enable_if_t<std::is_same_v<double, T>>>
	const double* data() const
	{
		return xmm.m256d_f64;
	}

	template<typename check = std::enable_if_t<std::is_same_v<int32_t, T>>>
	const int32_t* data() const
	{
		return xmm.m128i_i32;
	}

	template<typename check = std::enable_if_t<std::is_same_v<uint32_t, T>>>
	const uint32_t* data() const
	{
		return xmm.m128i_u32;
	}



	inline DVector operator+(const DVector& v) const
	{
		return _mm_add_t<T, __mT>(xmm, v.xmm);
	}

	inline DVector & operator+=(const DVector& v)
	{
		xmm = _mm_add_t<T, __mT>(xmm, v.xmm);
		return *this;
	}

	inline DVector operator-(const DVector& v) const
	{
		return _mm_sub_t<T, __mT>(xmm, v.xmm);
	}

	inline DVector& operator-=(const DVector& v)
	{
		xmm = _mm_sub_t<T, __mT>(xmm, v.xmm);
		return *this;
	}

	inline DVector operator*(T s) const
	{
		DVector sVector = DVector<T, Size>(s);
		return (*this)*sVector;
	}

	inline DVector& operator*=(T s)
	{
		DVector sVector = DVector<T, Size>(s);
		xmm = (*this)*sVector;
		return (*this);
	}

	inline DVector operator*(const DVector& v) const
	{
		return _mm_mul_t<T, __mT>(xmm, v.xmm);
	}

	inline DVector& operator*=(const DVector& v)
	{
		xmm = (*this)*v;
		return (*this);
	}

	DVector operator/(T s) const
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		assert(s != 0);
		return (*this) / DVector<T, Size>(s);
	}

	DVector& operator/=(T s)
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		assert(s != 0);
		xmm = (*this) / DVector<T, Size>(s);
		return *this;
	}

	inline DVector operator/(const DVector& v2) const
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		return _mm_div_t<T>(xmm, v2.xmm);
	}

	inline DVector& operator/=(const DVector& v2)
	{
		static_assert(!std::is_same<int32_t, T>() &&
					  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

		xmm = _mm_div_t<T>(xmm, v2.xmm);
		return *this;
	}


	DVector operator-() const
	{
		return (*this)*(DVector(-1));
	}


	inline T operator[](uint32_t i) const
	{
		assert(i < Size);
		return (data())[i];
	}

	inline T& operator[](uint32_t i)
	{
		assert(i < Size);
		return (data())[i];
	}

	void store(T* mem) const
	{
		_mm_storea_t<T, __mT>(mem, xmm);
	}
	
	void insert(uint8_t pos, T value)
	{
		xmm = _mm_insert_t(xmm, value, pos);
	}
	
	void setX(T value) { insert(0, value); }

	template<typename check = std::enable_if_t<Size >= 2>>
	void setY(T value) { insert(1, value); }

	template<typename check = std::enable_if_t<Size >= 3>>
	void setZ(T value) { insert(2, value); }

	template<typename check = std::enable_if_t<Size >= 4>>
	void setW(T value) { insert(3, value); }

	inline T x() const
	{
		return (*this)[0];
	}

	template<typename = std::enable_if_t<Size >= 2>>
	inline T y() const
	{
		return (*this)[1];
	}

	template<typename = std::enable_if_t<Size >= 3>>
	inline T z() const
	{
		return (*this)[2];
	}

	template<typename = std::enable_if_t<Size >= 4>>
	inline T w() const
	{
		return (*this)[3];
	}

	inline T& x()
	{
		return (*this)[0];
	}

	template<typename = std::enable_if_t<Size >= 2>>
	inline T& y()
	{
		return (*this)[1];
	}

	template<typename = std::enable_if_t<Size >= 3>>
	inline T& z()
	{
		return (*this)[2];
	}

	template<typename = std::enable_if_t<Size >= 4>>
	inline T& w()
	{
		return (*this)[3];
	}

	uint8_t size() const
	{
		return Size;
	}

	T length2() const
	{
		return dot(*this, *this);
	}

	T length() const
	{
		return std::sqrt(dot(*this, *this));
	}

	inline T minComponent()
	{
		T m = (*this)[0];
		for (uint8_t i = 1; i < Size; ++i)
		{
			if (m > (*this)[i])
			{
				m = (*this)[i];
			}
		}
		return m;
	}

	inline T maxComponent()
	{
		T m = (*this)[0];
		for (uint8_t i = 1; i < Size; ++i)
		{
			if (m < (*this)[i])
			{
				m = (*this)[i];
			}
		}
		return m;
	}

	inline bool operator==(const DVector& v2) const
	{
		DVector<T, Size> res = _mm_cmp_eq_t<T>(xmm, v2.xmm);
		for (uint8_t i = 0; i < Size; ++i)
		{
			uint32_t k = *((uint32_t*)(&res[i]));
			if (k == (uint32_t)0)
			{
				return false;
			}
		}
		return true;
	}

	template<uint8_t controlValue>
	inline DVector permute() const
	{
		return _mm_permute_ts<controlValue, T>::f(xmm); 
	}
public:


	template<uint8_t rSize = Size>
	static inline DVector abs(const DVector& v)
	{
		DVector res;
		for (uint8_t i = 0; i < rSize; ++i)
		{
			res[i] = std::abs(v[i]);
		}
		return res;
	}

	template<uint8_t rSize = Size>
	static inline T dot(const DVector& v1, const DVector& v2)
	{
		DVector res = v1*v2;

		T resDot = 0;
		for (uint8_t i = 0; i < rSize; ++i)
		{
			resDot += res[i];
		}
		return resDot;
	}

	static inline T absDot(const DVector& v1, const DVector& v2)
	{
		return std::abs(dot(v1, v2));
	}

	template<typename sizeCheck = std::enable_if_t<Size >= 2>>
	static inline DVector cross(const DVector& v1, const DVector& v2)
	{
		DVector<T, Size> v1p = _mm_permute_ts<0b11001001, T>::f(v1.xmm); // Y Z X W
		DVector<T, Size> v2p = _mm_permute_ts<0b11010010, T>::f(v2.xmm); // Z X Y W

		DVector<T, Size> v3 = v1p*v2p;
		v1p  = _mm_permute_ts<0b11001001, T>::f(v1p.xmm); // Z X Y W
		v2p  = _mm_permute_ts<0b11010010, T>::f(v2p.xmm); // Y Z X W

		DVector<T, Size> v4 = v1p*v2p;
		return v3-v4;
	}

	static inline DVector lerp(const DVector& v1, const DVector& v2, float t)
	{
		float tInv = 1.0 - t;
		return mAdd(v1, tInv, v2 * t);
	}

	static inline DVector mAdd(const DVector& v1, float s, const DVector& v2)
	{
		return mAdd(v1, DVector(s), v2);
	}

	static inline DVector mAdd(const DVector& v1, const DVector& v2, const DVector& v3)
	{
		return _mm_madd_t<T>(v1.xmm, v2.xmm, v3.xmm);
	}

	static inline DVector normalize(const DVector& v)
	{
		return v / v.length();
	}

	static inline DVector minVector(const DVector& v1, const DVector& v2)
	{
		return _mm_min_t<T>(v1.xmm, v2.xmm);
	}

	static inline DVector maxVector(const DVector& v1, const DVector& v2)
	{
		return _mm_max_t<T>(v1.xmm, v2.xmm);
	}

	static inline void makeBasisByVector(const DVector& v1, DVector& v2, DVector& v3)
	{

		if (std::abs(v1.x()) > std::abs(v1.y()))
		{
			v2 = DVector(-v1.z(), 0, v1.x());
		}
		else
		{
			v2 = DVector(0, v1.z(), -v1.y());
		}

		v2 = v2.normalize(v2);;
		v3 = cross(v1, v2);
	}

protected:
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