#pragma once
#include "stdafx.h"
#include "TypedSSE.h"
#include "DVector.h"

WML_BEGIN

using namespace TypedSSE;
template<typename T>
struct dmatrix_size
{};

template<>
struct dmatrix_size<float>
{
	static constexpr uint8_t size = 2;
};

template<>
struct dmatrix_size<int32_t>
{
	static constexpr uint8_t size = 2;
};

template<>
struct dmatrix_size<uint32_t>
{
	static constexpr uint8_t size = 2;
};

template<>
struct dmatrix_size<double>
{
	static constexpr uint8_t size = 4;
};

template<typename T>
constexpr uint8_t dmatrix_size_v = dmatrix_size<T>::size;

template<typename T, typename __mT = __m256_t<T>, uint8_t Size=dmatrix_size_v<T>, uint8_t alignment=sse_alignment_size_v<__mT>>
class alignas(alignment) DMatrix
{
public:
	// if float/int32_t/uint32_t - every row = Column(i)|Column(i+1)
	// if double - every row = Column(i)
	__mT xmm[Size];

	DMatrix(T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13,
			T m20, T m21, T m22, T m23,
			T m30, T m31, T m32, T m33
	)
	{
		loadData(m00, m01, m02, m03,
				 m10, m11, m12, m13,
				 m20, m21, m22, m23,
				 m30, m31, m32, m33);
	}

	DMatrix()
	{
		loadData(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0);
	}

	template<typename T2 = T, typename  std::enable_if_t<std::is_same_v<T2, double>, bool> = true>
	void loadData(T m00, T m01, T m02, T m03,
				   T m10, T m11, T m12, T m13,
				   T m20, T m21, T m22, T m23,
				   T m30, T m31, T m32, T m33)
	{
		xmm[0] = _mm_setr_t<T>(m00, m10, m20, m30);
		xmm[1] = _mm_setr_t<T>(m01, m11, m21, m31);
		xmm[2] = _mm_setr_t<T>(m02, m12, m22, m32);
		xmm[3] = _mm_setr_t<T>(m03, m13, m23, m33);
	}

	template<typename T2 = T, typename std::enable_if_t<!std::is_same_v<T2, double>, bool> = true>
	void loadData(T m00, T m01, T m02, T m03,
				  T m10, T m11, T m12, T m13,
				  T m20, T m21, T m22, T m23,
				  T m30, T m31, T m32, T m33)
	{
		xmm[0] = _mm_setr_t<T>(m00, m10, m20, m30, m01, m11, m21, m31);
		xmm[1] = _mm_setr_t<T>(m02, m12, m22, m32, m03, m13, m23, m33);
	}

	inline DMatrix operator+(const DMatrix& v) const
	{
		DMatrix res;
		add(*this, v, res);
		return res;
	}

	inline DMatrix& operator+=(const DMatrix& v)
	{
		add(*this, v, *this);
		return *this;
	}

	inline DMatrix operator-(const DMatrix& v) const
	{
		DMatrix res;
		sub(*this, v, res);
		return res;
	}

	inline DMatrix& operator-=(const DMatrix& v)
	{
		sub(*this, v, *this);
		return *this;
	}

	inline DMatrix operator*(const DMatrix& v) const
	{
		DMatrix res;
		multMatrix(*this, v, res);
		return res;
	}

	inline DMatrix& operator*=(const DMatrix& v)
	{
		DMatrix res;
		multMatrix(*this, v, res);
		return res;
	}

	inline DMatrix operator*(T s) const
	{
		DMatrix res;
		multScalar(*this, s, res);
		return res;
	}

	inline DMatrix& operator*=(T s)
	{
		multScalar(*this, s, *this);
		return (*this);
	}

	template<typename T2>
	inline void setTransition(const DVector<T2, 3>& trans)
	{
		setTransition(trans.x(), trans.y(), trans.z());
	}

	inline void setTransition(T x, T y, T z)
	{
		(*this)[0][3] = x; (*this)[1][3] = y; (*this)[2][3] = z;
	}

	template<typename T2>
	inline void transition(const DVector<T2, 3>& trans)
	{
		transition(trans.x(), trans.y(), trans.z());
	}

	inline void transition(float x, float y, float z)
	{
		(*this)[0][3] += x; (*this)[1][3] += y; (*this)[2][3] += z;
	}
	
	template<typename T2>
	inline void setScale(const DVector<T2, 3>& scale)
	{
		setScale(scale.x(), scale.y(), scale.z());
	}

	inline void setScale(float x, float y, float z)
	{
		(*this)[0][0] = x; (*this)[1][1] = y; (*this)[3][3] = z;
	}

	inline const T* operator[](uint32_t i) const
	{
		assert(i < 4);
		return &((data())[i*4]);
	}

	inline T* operator[](uint32_t i)
	{
		assert(i < 4);
		return &((data())[i*4]);
	}

	inline T* data() noexcept
	{
		return (T*)(&xmm);
	}

	inline const T* data() const
	{
		return (T*)(&xmm);
	}
	
	inline void transpose() noexcept
	{
		// pre-store data 
		for (size_t i = 0; i < Size; ++i)
		{
			_mm_storea_t<T, __mT>((T*)(&xmm[i]), xmm[i]);
		}
		

		for (uint8_t i = 0; i < 4; ++i)
		{
			for (uint8_t j = i + 1; j < 4; ++j)
			{
				T t = (*this)[i][j];
				(*this)[i][j] = (*this)[j][i];
				(*this)[j][i] = t;
			}
		}
	}


	static inline DMatrix transpose(const DMatrix& m) noexcept
	{
		DMatrix res = m;

		// pre-store data 
		for (size_t i = 0; i < Size; ++i)
		{
			_mm_storea_t<T, __mT>((T*)(&res.xmm[i]), res.xmm[i]);
		}

		for (uint8_t i = 0; i < 4; ++i)
		{
			for (uint8_t j = i + 1; j < 4; ++j)
			{
				T t = res[i][j];
				res[i][j] = res[j][i];
				res[j][i] = t;
			}
		}
		return res;
	}
private:
	template<typename T2=T, typename  std::enable_if_t<std::is_same_v<T2, double>, bool> = true>
	friend inline DVector<T, 4> operator*(const DVector<T, 4>& v1, const DMatrix& m0) noexcept
	{
		T resData[4];

		for (uint8_t i = 0; i < Size; ++i)
		{
			DVector<T, 4> v2 = m0.xmm[i];
			DVector<T, 4> vres = v1 * v2;
			
			T res = 0;
			for (uint8_t j = 0; j < 4; ++j)
			{
				res += vres[j];
			}
			resData[i] = res;
		}


		return DVector<T, 4>(resData);
	}

	template<typename T2 = T, typename  std::enable_if_t<std::is_same_v<T2, double>, bool> = true>
	friend inline DVector<T, 3> operator*(const DVector<T, 3>& v1, const DMatrix& m0) noexcept
	{
		T resData[4];

		for (uint8_t i = 0; i < 3; ++i)
		{
			DVector<T, 3> v2 = m0.xmm[i];
			DVector<T, 3> vres = v1 * v2;

			T res = 0;
			for (uint8_t j = 0; j < 3; ++j)
			{
				res += vres[j];
			}
			resData[i] = res;
		}

		resData[3] = 0;

		return DVector<T, 3>(resData);
	}


	template<typename T2 = T, typename  std::enable_if_t<!std::is_same_v<T2, double>, bool> = true>
	friend inline DVector<T, 4> operator*(const DVector<T, 4>& v1, const DMatrix& m0) noexcept
	{
		T resData[4];

		__mT xmmV1 = _mm256_broadcast_t<T>((T*)(&(v1.xmm)));
		for (uint8_t i = 0; i < Size; ++i)
		{
			__mT xmmRes = _mm_mul_t<T, __mT>(xmmV1, m0.xmm[i]);

			T res0 = 0;
			T res1 = 0;
			for (uint8_t j = 0; j < 4; ++j)
			{
				res0 += ((T*)(&xmmRes))[j];
				res1 += ((T*)(&xmmRes))[j + 4];
			}
			resData[i*2] = res0;
			resData[i*2+1] = res1;
		}

		return DVector<T, 4>(resData);
	}

	template<typename T2 = T, typename  std::enable_if_t<!std::is_same_v<T2, double>, bool> = true>
	friend inline DVector<T, 3> operator*(const DVector<T, 3>& v1, const DMatrix& m0) noexcept
	{
		T resData[4];

		__mT xmmV1 = _mm256_broadcast_t<T>((T*)(&(v1.xmm)));
		for (uint8_t i = 0; i < Size; ++i)
		{
			__mT xmmRes = _mm_mul_t<T, __mT>(xmmV1, m0.xmm[i]);

			T res0 = 0;
			for (uint8_t j = 0; j < 3; ++j)
			{
				res0 += ((T*)(&xmmRes))[j];
			}
			resData[i * 2] = res0;
		}

		resData[3] = 0;

		return DVector<T, 3>(resData);
	}


	template<typename T, typename = std::enable_if_t<VSize == 3>>
	static inline DVector<T, 3> faceforward(const DVector<T, 3> &goalV, const DVector<T, 3>& v2)
	{
		return (dot(goalV, v2) < 0) ? -goalV : goalV;
	}

	static inline void add(const DMatrix& m0, const DMatrix& m1, DMatrix& mres) noexcept
	{
		for (uint8_t i = 0; i < Size; ++i)
		{
			mres.xmm[i] = _mm_add_t<T, __mT>(m0.xmm[i], m1.xmm[i]);
		}
	}

	static inline void sub(const DMatrix& m0, const DMatrix& m1, DMatrix& mres) noexcept
	{
		for (uint8_t i = 0; i < Size; ++i)
		{
			mres.xmm[i] = _mm_sub_t<T, __mT>(m0.xmm[i], m1.xmm[i]);
		}
	}

	static inline void multMatrix(const DMatrix& m0, const DMatrix& m1,  DMatrix& mres) noexcept
	{
		T rowData[4];
		// pre-store data 
		for (size_t i = 0; i < Size; ++i)
		{
			_mm_storea_t<T, __mT>((T*)(&m0.xmm[i]), m0.xmm[i]);
		}

		for (size_t i = 0; i < 4; ++i) // rows in m0
		{
			for (size_t j = 0; j < Size; ++j) // columns in m1
			{
				rowData[0] = m0[0][i]; 
				rowData[1] = m0[1][i];
				rowData[2] = m0[2][i];
				rowData[3] = m0[3][i];

				__mT rowXmm = _mm256_broadcast_t<T>(rowData);
				rowXmm  = _mm_mul_t<T, __mT>(rowXmm, m1.xmm[j]);

				if constexpr(Size ==  2) // if 2 columns in one register
				{
					T res0 = 0;
					T res1 = 0;
					for (size_t k = 0; k < 4; ++k)
					{
						res0 += ((T*)(&rowXmm))[k];
						res1 += ((T*)(&rowXmm))[k+4];
					}
					mres[j * 2][i] = res0;
					mres[j * 2+1][i] = res1;
				}
				
				if constexpr(Size == 4)
				{ // Size == 4 if 1 column in one register
					T res = 0;
					for (size_t k = 0; k < 4; ++k)
					{
						res += ((T*)(&rowXmm))[k];
					}
					mres[j][i] = res;
				}
			}
		}
	}

	static inline void multScalar(const DMatrix& m0, T scalar, DMatrix& mres) noexcept
	{
		__mT xmmS = _mm_set1_t<T, __mT>(scalar);

		for (size_t j = 0; j < Size; ++j) // columns in m1
		{
			mres.xmm[j] = _mm_mul_t<T, __mT>(xmmS, m0.xmm[j]);
		}
	}
};

using DMatrixf = DMatrix<float>;
using DMatrixd = DMatrix<double>;
using DMatrixi = DMatrix<int32_t>;
using DMatrixu = DMatrix<uint32_t>;

WML_END


