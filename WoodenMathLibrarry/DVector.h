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

	template<TTNumbrEqual(Size, 4)>
	DVector(T x, T y, T z, T w)
	{
		xmm = _mm_setr_t<T>(x, y, z, w);
	}

	template<TTNumbrEqual(Size, 3)>
	DVector(T x, T y, T z)
	{
		xmm = _mm_setr_t<T>(x, y, z, 0);
	}

	template<TTNumbrEqual(Size, 2)>
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

	template<TTNumbrEqual(Size, 4)>
	explicit DVector(const DVector<T, 3>& v, T w=0)
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, w);
	}

	template<TTNumbrEqual(Size, 4)>
	explicit DVector(DVector<T, 3>&& v, T w = 0):
		xmm(std::move(v.xmm))
	{
		insert(3, w);
	}


	template<TTNumbrEqual(Size, 3)>
	explicit DVector(const DVector<T, 4>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		insert(3, 0);
	}


	template<TTNumbrEqual(Size, 3)>
	explicit DVector(DVector<T, 4>&& v):
		xmm(std::move(v.xmm))
	{
		insert(3, 0);
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

	template<uint8_t LSize = Size>
	T length2() const
	{
		return dot<LSize>(*this, *this);
	}

	template<uint8_t LSize = Size>
	T length() const
	{
		return std::sqrt(dot<LSize>(*this, *this));
	}

	
	inline bool operator==(const DVector& v2) const
	{
		for (uint8_t i = 0; i < Size; ++i)
		{
			if ((*this)[i] != v2[i])
			{
				return false;
			}
		}
		return true;
	}

	inline bool operator!=(const DVector& v2) const
	{
		return !((*this) == v2);
	}

	void normalize()
	{
		(*this) /= this->length();
	}

	template<uint8_t controlValue>
	inline DVector permute() const
	{
		return _mm_permute_ts<controlValue, T>::f(xmm); 
	}

	bool bIsAllZero()
	{
		return 
	}
};

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> abs(const DVector<VT, VSize>& v)
{
	DVector<VT, VSize> res;
	for (uint8_t i = 0; i < VSize; ++i)
	{
		res[i] = std::abs(v[i]);
	}
	return res;
}

template<uint8_t DotSize, typename VT, uint8_t VSize>
inline VT dot(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	DVector<VT, VSize> res = v1 * v2;

	VT resDot = 0;
	for (uint8_t i = 0; i < DotSize; ++i)
	{
		resDot += res[i];
	}
	return resDot;
}

template<typename VT, uint8_t VSize>
inline VT dot(const DVector<VT,VSize>& v1, const DVector<VT, VSize>& v2)
{
	return dot<VSize, VT, VSize>(v1, v2);
}



template<typename VT, uint8_t VSize>
inline VT absDot(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return std::abs(dot(v1, v2));
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> cross(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	DVector<T, Size> v1p = _mm_permute_ts<0b11001001, T>::f(v1.xmm); // Y Z X W
	DVector<T, Size> v2p = _mm_permute_ts<0b11010010, T>::f(v2.xmm); // Z X Y W

	DVector<T, Size> v3 = v1p * v2p;
	v1p = _mm_permute_ts<0b11001001, T>::f(v1p.xmm); // Z X Y W
	v2p = _mm_permute_ts<0b11010010, T>::f(v2p.xmm); // Y Z X W

	DVector<T, Size> v4 = v1p * v2p;
	return v3 - v4;
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> lerp(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, const DVector<VT, VSize>& t)
{
	DVector<VT, VSize> tInv = DVector<VT, VSize>(1) - t;
	return mad(v1, tInv, v2 * t);
}

template<typename VT, uint8_t VSize>
inline VT minComponent(const DVector<VT, VSize>& v) 
{
	T m = v[0];
	for (uint8_t i = 1; i < Size; ++i)
	{
		if (m > v[i])
		{
			m = v[i];
		}
	}
	return m;
}

template<typename VT, uint8_t VSize>
void permute(const DVector<VT, VSize> &v, const DVector<uint8_t, VSize>& indices)
{
	DVector<VT, VSize> res;
	for (uint8_t i = 0; i < VSize; i++)
	{
		res[i] = v[indices[i]];
	}
	return res;
}

template<typename VT, uint8_t VSize>
inline uint8_t maxDimension(const DVector<VT, VSize>& v)
{
	VT m = v[0];
	uint8_t iM = 0;
	for (uint8_t i = 1; i < Size; ++i)
	{
		if (m < v[i])
		{
			m = v[i];
			iM = i;
		}
	}
	return iM;
}

template<typename VT, uint8_t VSize>
inline VT maxComponent(const DVector<VT, VSize>& v)
{
	VT m = v[0];
	for (uint8_t i = 1; i < Size; ++i)
	{
		if (m < v[i])
		{
			m = v[i];
		}
	}
	return m;
}


template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> lerp(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, float t)
{
	float tInv = 1.0 - t;
	return mad(v1, tInv, v2 * t);
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> mad(const DVector<VT, VSize>& v1, float s, const DVector<VT, VSize>& v2)
{
	return mad(v1, DVector<VT, VSize>(s), v2);
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> mad(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, const DVector<VT, VSize>& v3)
{
	return _mm_madd_t<T>(v1.xmm, v2.xmm, v3.xmm);
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> normalize(const DVector<VT, VSize>& v)
{
	return v / v.length();
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> minVector(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return _mm_min_t<T>(v1.xmm, v2.xmm);
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> maxVector(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return _mm_max_t<T>(v1.xmm, v2.xmm);
}
template<typename VT, uint8_t VSize>
inline void makeBasisByVector(const DVector<VT, VSize>& v1, DVector<VT, VSize>& v2, DVector<VT, VSize>& v3)
{

	if (std::abs(v1.x()) > std::abs(v1.y()))
	{
		v2 = DVector<VT, VSize>(-v1.z(), 0, v1.x());
	}
	else
	{
		v2 = DVector<VT, VSize>(0, v1.z(), -v1.y());
	}

	v2 = v2.normalize(v2);;
	v3 = cross(v1, v2);
}


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

using DVector2u8 = DVector<uint8_t, 2>;
using DVector3u8 = DVector<uint8_t, 3>;
using DVector4u8 = DVector<uint8_t, 4>;

WML_END