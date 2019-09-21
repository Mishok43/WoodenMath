#pragma once
#include "stdafx.h"
#include "TypedSSE.h"

WML_BEGIN

// TODO: fork some methods for different sizes 

using namespace TypedSSE;


template<typename T, uint8_t Size, typename __mT = __m_t<T, (Size+3)/4>, uint8_t alignment = sse_alignment_size_v<__mT>>
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

	template<typename T2>
	operator DVector<T2, Size>() const
	{
		DVector<T2, Size> res;
		for (uint8_t i = 0; i < Size; i++)
		{
			res[i] = (T2)((*this)[i]);
		}
		return res;
	}


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
		for (uint8_t i = 0; i < Size; ++i)
		{
			if ((*this)[i] != 0)
			{
				return false;
			}
		}
		return true;
	}

public:
	static DVector abs(const DVector& v)
	{
		DVector res;
		for (uint8_t i = 0; i < Size; ++i)
		{
			res[i] = std::abs(v[i]);
		}
		return res;
	}

	template<uint8_t DotSize>
	static T dot(const DVector& v1, const DVector& v2)
	{
		DVector res = v1 * v2;

		T resDot = 0;
		for (uint8_t i = 0; i < DotSize; ++i)
		{
			resDot += res[i];
		}
		return resDot;
	}

	
	static T dot(const DVector& v1, const DVector& v2)
	{
		return dot<Size>(v1, v2);
	}

	static T clamp(const DVector& v, float low, float high = std::numeric_limits<float>())
	{
		for (uint8_t i = 0; i < Size; i++)
		{
			v[i] = std::clamp(low, high);
		}
		return v;
	}

	static T absDot(const DVector& v1, const DVector& v2)
	{
		return std::abs(dot(v1, v2));
	}

	static DVector cross(const DVector& v1, const DVector& v2)
	{
		DVector v1p = _mm_permute_ts<0b11001001, T>::f(v1.xmm); // Y Z X W
		DVector v2p = _mm_permute_ts<0b11010010, T>::f(v2.xmm); // Z X Y W

		DVector v3 = v1p * v2p;
		v1p = _mm_permute_ts<0b11001001, T>::f(v1p.xmm); // Z X Y W
		v2p = _mm_permute_ts<0b11010010, T>::f(v2p.xmm); // Y Z X W

		DVector v4 = v1p * v2p;
		return v3 - v4;
	}

	static DVector lerp(const DVector& v1, const DVector& v2, const DVector& t)
	{
		DVector tInv = DVector(1) - t;
		return mad(v1, tInv, v2 * t);
	}

	
	static T minComponent(const DVector& v)
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


	static bool isSameHemisphere(const DVector &w, const DVector &wp)
	{
		return w.z * wp.z > 0;
	}

	void permute(const DVector &v, const DVector& indices)
	{
		DVector res;
		for (uint8_t i = 0; i < Size; i++)
		{
			res[i] = v[indices[i]];
		}
		return res;
	}

	static uint8_t maxDimension(const DVector& v)
	{
		T m = v[0];
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

	
	static T maxComponent(const DVector& v)
	{
		T m = v[0];
		for (uint8_t i = 1; i < Size; ++i)
		{
			if (m < v[i])
			{
				m = v[i];
			}
		}
		return m;
	}

	
	static T ceil(const DVector& v)
	{
		DVector r;
		for (uint8_t i = 0; i < Size; i++)
		{
			r[i] = std::ceil(v[i]);
		}
		return r;
	}

	
	static T floor(const DVector& v)
	{
		DVector r;
		for (uint8_t i = 0; i < Size; i++)
		{
			r[i] = std::floor(v[i]);
		}
		return r;
	}

	
	static DVector reflect(const DVector& wo, const DVector& n)
	{
		return -wo + 2 * dot(wo, n)*n;
	}



	
	static DVector sqrt(const DVector& v)
	{
		return _mm_sqrt_t(v);
	}

	
	static DVector lerp(const DVector& v1, const DVector& v2, float t)
	{
		float tInv = 1.0 - t;
		return mad(v1, tInv, v2 * t);
	}

	
	static DVector mad(const DVector& v1, float s, const DVector& v2)
	{
		return mad(v1, DVector(s), v2);
	}
	
	static DVector mad(const DVector& v1, const DVector& v2, const DVector& v3)
	{
		return _mm_madd_t<T>(v1.xmm, v2.xmm, v3.xmm);
	}
	
	static DVector normalize(const DVector& v)
	{
		return v / v.length();
	}
	
	static DVector minv(const DVector& v1, const DVector& v2)
	{
		return _mm_min_t<T>(v1.xmm, v2.xmm);
	}
	
	static DVector maxv(const DVector& v1, const DVector& v2)
	{
		return _mm_max_t<T>(v1.xmm, v2.xmm);
	}



	
	static void makeBasisByVector(const DVector& v1, DVector& v2, DVector& v3)
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

};




template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> abs(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::abs(v);
}

template<uint8_t DotSize, typename VT, uint8_t VSize>
inline VT dot(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::dot<DotSize>(v1, v2);
}

template<typename VT, uint8_t VSize>
inline VT dot(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::dot(v1, v2);
}

template<typename VT, uint8_t VSize>
inline VT clamp(const DVector<VT, VSize>& v, float low, float high = std::numeric_limits<float>())
{
	return DVector<VT, VSize>::clamp(v, low, high);
}

template<typename VT, uint8_t VSize>
inline VT absDot(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::absDot(v1, v2);
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> cross(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::cross(v1, v2);
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> lerp(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, const DVector<VT, VSize>& t)
{
	return DVector<VT, VSize>::lerp(v1, v2, t);
}

template<typename VT, uint8_t VSize>
inline VT minComponent(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::minComponent(v);
}

template<typename VT, uint8_t VSize>
inline bool isSameHemisphere(const DVector<VT, VSize> &w, const DVector<VT, VSize> &wp)
{
	return DVector<VT, VSize>::isSameHemisphere(w, wp);
}

template<typename VT, uint8_t VSize>
void permute(const DVector<VT, VSize> &v, const DVector<uint8_t, VSize>& indices)
{
	return DVector<VT, VSize>::permute(v, indices);
}

template<typename VT, uint8_t VSize>
inline uint8_t maxDimension(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::maxDimension(v);
}

template<typename VT, uint8_t VSize>
inline VT maxComponent(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::maxComponent(v);
}

template<typename VT, uint8_t VSize>
inline VT ceil(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::ceil(v);
}

template<typename VT, uint8_t VSize>
inline VT floor(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::floor(v);
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> reflect(const DVector<VT, VSize>& wo, const DVector<VT, VSize>& n)
{
	return DVector<VT, VSize>::reflect(wo, n);
}



template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> sqrt(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::sqrt(v);
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> lerp(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, float t)
{
	return DVector<VT, VSize>::lerp(v1, v2, t);
}

template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> mad(const DVector<VT, VSize>& v1, float s, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::mad(v1, s, v2);
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> mad(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, const DVector<VT, VSize>& v3)
{
	return DVector<VT, VSize>::mad(v1, v2, v3);
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> normalize(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::normalize(v);
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> minv(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::minv(v1, v2);
}
template<typename VT, uint8_t VSize>
inline DVector<VT, VSize> maxv(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::maxv(v1, v2);
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



using DVector2f = typename DVector<float, 2>;
using DVector3f = typename  DVector<float, 3>;
using DVector4f = typename  DVector<float, 4>;
using DVector8f = typename  DVector<float, 8>;

using DVector2d = typename  DVector<double, 2>;
using DVector3d = typename  DVector<double, 3>;
using DVector4d = typename  DVector<double, 4>;

using DVector2i = typename  DVector<int32_t, 2>;
using DVector3i = typename  DVector<int32_t, 3>;
using DVector4i = typename  DVector<int32_t, 4>;

using DVector2u = typename  DVector<uint32_t, 2>;
using DVector3u = typename  DVector<uint32_t, 3>;
using DVector4u = typename  DVector<uint32_t, 4>;
WML_END