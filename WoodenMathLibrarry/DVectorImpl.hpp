#pragma once
#include "DVector.h"

WML_BEGIN

template<typename T, uint8_t Size>
DVector<T, Size>::DVector<T, Size>(T* data)
{
	xmm = _mm_loadu_t(data);
}

template<typename T, uint8_t Size>
DVector(T broadcastValue = 0)
{
	xmm = _mm_set1_t<T, __mT>(broadcastValue);
}

template<typename T, uint8_t Size, TTNumbrEqual(Size, 4)>
DVector(T x, T y, T z, T w)
{
	xmm = _mm_setr_t<T>(x, y, z, w);
}

template<typename T, uint8_t Size, TTNumbrEqual(Size, 3)>
DVector(T x, T y, T z)
{
	xmm = _mm_setr_t<T>(x, y, z, 0);
}

template<typename T, uint8_t Size, TTNumbrEqual(Size, 2)>
DVector(T x, T y)
{
	xmm = _mm_setr_t<T>(x, y, 0, 0);
}

template<typename T, uint8_t Size>
DVector(const __m_t<T>& data) :
	xmm(data)
{
}

template<typename T, uint8_t Size>
DVector(__m_t<T>&& data) :
	xmm(std::move(data))
{
}

template<typename T, uint8_t Size, typename T2>
operator DVector<T2, Size>() const
{
	DVector<T2, Size> res;
	for (uint8_t i = 0; i < Size; i++)
	{
		res[i] = (T2)((*this)[i]);
	}
	return res;
}


template<typename T, uint8_t Size, TTNumbrEqual(Size, 4)>
explicit DVector(const DVector<T, 3>& v, T w = 0)
{
	_mm_loada_t<T>(&v.xmm);
	insert(3, w);
}

template<typename T, uint8_t Size, TTNumbrEqual(Size, 4)>
explicit DVector(DVector<T, 3>&& v, T w = 0) :
	xmm(std::move(v.xmm))
{
	insert(3, w);
}


template<typename T, uint8_t Size, TTNumbrEqual(Size, 3)>
explicit DVector(const DVector<T, 4>& v)
{
	_mm_loada_t<T>(&v.xmm);
	insert(3, 0);
}


template<typename T, uint8_t Size, TTNumbrEqual(Size, 3)>
explicit DVector(DVector<T, 4>&& v) :
	xmm(std::move(v.xmm))
{
	insert(3, 0);
}


template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<__mT, __m128>>>
operator __m128() const
{
	return xmm;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<__mT, __m128i>>>
operator __m128i() const
{
	return xmm;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<__mT, __m256d>>>
operator __m256d() const
{
	return xmm;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<T, float>>>
float* data()
{
	return xmm.m128_f32;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<T, double>>>
double* data()
{
	return xmm.m256d_f64;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<T, int32_t>>>
int32_t* data()
{
	return xmm.m128i_i32;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<T, uint32_t>>>
uint32_t* data()
{
	return xmm.m128i_u32;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<float, T>>>
const float* data() const
{
	return xmm.m128_f32;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<double, T>>>
const double* data() const
{
	return xmm.m256d_f64;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<int32_t, T>>>
const int32_t* data() const
{
	return xmm.m128i_i32;
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<std::is_same_v<uint32_t, T>>>
const uint32_t* data() const
{
	return xmm.m128i_u32;
}


template<typename T, uint8_t Size>
inline bool operator==(T value)
{
	for (uint8_t i = 0; i < Size; i++)
	{
		if ((*this)[i] != value)
		{
			return false;
		}
	}

	return true;
}

template<typename T, uint8_t Size>
inline DVector operator+(const DVector& v) const
{
	return _mm_add_t<T, __mT>(xmm, v.xmm);
}

template<typename T, uint8_t Size>
inline DVector & operator+=(const DVector& v)
{
	xmm = _mm_add_t<T, __mT>(xmm, v.xmm);
	return *this;
}

template<typename T, uint8_t Size>
inline DVector operator-(const DVector& v) const
{
	return _mm_sub_t<T, __mT>(xmm, v.xmm);
}

template<typename T, uint8_t Size>
inline DVector& operator-=(const DVector& v)
{
	xmm = _mm_sub_t<T, __mT>(xmm, v.xmm);
	return *this;
}

template<typename T, uint8_t Size>
inline DVector operator*(T s) const
{
	DVector sVector = DVector<T, Size>(s);
	return (*this)*sVector;
}

template<typename T, uint8_t Size>
inline DVector& operator*=(T s)
{
	DVector sVector = DVector<T, Size>(s);
	xmm = (*this)*sVector;
	return (*this);
}

template<typename T, uint8_t Size>
inline DVector operator*(const DVector& v) const
{
	return _mm_mul_t<T, __mT>(xmm, v.xmm);
}

template<typename T, uint8_t Size>
inline DVector& operator*=(const DVector& v)
{
	xmm = (*this)*v;
	return (*this);
}
template<typename T, uint8_t Size>
DVector operator/(T s) const
{
	static_assert(!std::is_same<int32_t, T>() &&
				  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

	assert(s != 0);
	return (*this) / DVector<T, Size>(s);
}

template<typename T, uint8_t Size>
DVector& operator/=(T s)
{
	static_assert(!std::is_same<int32_t, T>() &&
				  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

	assert(s != 0);
	xmm = (*this) / DVector<T, Size>(s);
	return *this;
}

template<typename T, uint8_t Size>
inline DVector operator/(const DVector& v2) const
{
	static_assert(!std::is_same<int32_t, T>() &&
				  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

	return _mm_div_t<T>(xmm, v2.xmm);
}

template<typename T, uint8_t Size>
inline DVector& operator/=(const DVector& v2)
{
	static_assert(!std::is_same<int32_t, T>() &&
				  !std::is_same<uint32_t, T>(), "Division of int types is not supported");

	xmm = _mm_div_t<T>(xmm, v2.xmm);
	return *this;
}

template<typename T, uint8_t Size>
DVector operator-() const
{
	return (*this)*(DVector(-1));
}


template<typename T, uint8_t Size>
inline T operator[](uint32_t i) const
{
	assert(i < Size);
	return (data())[i];
}

template<typename T, uint8_t Size>
inline T& operator[](uint32_t i)
{
	assert(i < Size);
	return (data())[i];
}

template<typename T, uint8_t Size>
void store(T* mem) const
{
	_mm_storea_t<T, __mT>(mem, xmm);
}

template<typename T, uint8_t Size>
void insert(uint8_t pos, T value)
{
	xmm = _mm_insert_t(xmm, value, pos);
}

template<typename T, uint8_t Size>
void setX(T value)
{
	insert(0, value);
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<Size >= 2>>
void setY(T value)
{
	insert(1, value);
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<Size >= 3>>
void setZ(T value)
{
	insert(2, value);
}

template<typename T, uint8_t Size, typename check = std::enable_if_t<Size >= 4>>
void setW(T value)
{
	insert(3, value);
}

template<typename T, uint8_t Size>
inline T x() const
{
	return (*this)[0];
}

template<typename T, uint8_t Size, typename = std::enable_if_t<Size >= 2>>
inline T y() const
{
	return (*this)[1];
}

template<typename T, uint8_t Size, typename = std::enable_if_t<Size >= 3>>
inline T z() const
{
	return (*this)[2];
}

template< typename T, uint8_t Size, typename = std::enable_if_t<Size >= 4>>
inline T w() const
{
	return (*this)[3];
}

template<typename T, uint8_t Size>
inline T& x()
{
	return (*this)[0];
}

template<typename T, uint8_t Size, typename = std::enable_if_t<Size >= 2>>
inline T& y()
{
	return (*this)[1];
}

template<typename T, uint8_t Size, typename = std::enable_if_t<Size >= 3>>
inline T& z()
{
	return (*this)[2];
}

template<typename T, uint8_t Size, typename = std::enable_if_t<Size >= 4>>
inline T& w()
{
	return (*this)[3];
}


template<typename T, uint8_t Size>
uint8_t size() const
{
	return Size;
}

template<typename T, uint8_t Size, uint8_t LSize = Size>
T length2() const
{
	return dot<LSize>(*this, *this);
}

template<typename T, uint8_t Size, uint8_t LSize = Size>
T length() const
{
	return std::sqrt(dot<LSize>(*this, *this));
}
template<typename T, uint8_t Size>
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
template<typename T, uint8_t Size>
inline bool operator!=(const DVector& v2) const
{
	return !((*this) == v2);
}

template<typename T, uint8_t Size>
void normalize()
{
	(*this) /= this->length();
}

template<typename T, uint8_t Size, uint8_t controlValue>
inline DVector permute() const
{
	return _mm_permute_ts<controlValue, T>::f(xmm);
}

template<typename T, uint8_t Size>
bool  bIsAllZero()
{
	for (uint8_t i = 0; i < Size; ++i)
	{
		if ((*this)[i] != 0)
		{
			return false;
		}
	}
	return true;
}

WML_END