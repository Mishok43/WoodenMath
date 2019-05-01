#pragma once
#include "stdafx.h"
#include "TypedSSE.h"
#include "DVector.h"

WML_BEGIN

using namespace TypedSSE;

class alignas(32) DMatrixf
{
public:
	float m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33;

	DMatrixf(float m00, float m01, float m02, float m03,
			 float m10, float m11, float m12, float m13,
			 float m20, float m21, float m22, float m23,
			 float m30, float m31, float m32, float m33
	) :
		m00(m00), m01(m01), m02(m02), m03(m03),
		m10(m10), m11(m11), m12(m12), m13(m13),
		m20(m20), m21(m21), m22(m22), m23(m23),
		m30(m30), m31(m31), m32(m32), m33(m33)
	{}

	DMatrixf()
		: m00(1.0f), m11(1.0f), m22(1.0f), m33(1.0f)
	{}

	DMatrixf operator+(const DMatrixf& v) const
	{
		DMatrixf res;
		addition(*this, v, res);
		return res;
	}

	DMatrixf& operator+=(const DMatrixf& v)
	{
		addition(*this, v, *this);
		return *this;
	}

	DMatrixf operator-(const DMatrixf& v) const
	{
		DMatrixf res;
		substraction(*this, v, res);
		return res;
	}

	DMatrixf& operator-=(const DMatrixf& v)
	{
		substraction(*this, v, *this);
		return *this;
	}

	DMatrixf operator*(const DMatrixf& v) const
	{
		DMatrixf res;
		multiplication(*this, v, res);
		return res;
	}

	DMatrixf& operator*=(const DMatrixf& v)
	{
		DMatrixf res;
		multiplication(*this, v, res);
		return res;
	}

	template<typename T>
	void setTransition(const DVector<T, 3>& trans)
	{
		m30 = trans.x; m31 = trans.y; m32 = trans.z;
	}

	void setTransition(float x, float y, float z)
	{
		m30 = x; m31 = y; m32 = z;
	}

	void transition(const DVector<float, 3>& trans)
	{
		float m33Temp = m33;
		__m128 v0 = _mm_load_ps(data30());
		__m128 v1 = _mm_load_ps(trans.xmm());
		__m128 v2 = _mm_add_ps(v0, v1);
		_mm_store_ps(data30(), v2);
		m33 = m33Temp;
	}

	void transition(float x, float y, float z)
	{
		m30 += x; m31 += y; m32 += z;
	}

	template<typename T>
	void setScale(const DVector<T, 3>& scale)
	{
		m00 = scale.x; m11 = scale.y; m22 = scale.z;
	}

	void setScale(float x, float y, float z)
	{
		m00 = x; m11 = y; m22 = z;
	}

	void setRotationX()
private:
	static inline void addition(const DMatrixf& m0, const DMatrixf& m1, DMatrixf& mres) noexcept
	{
		__m256 v0 = _mm256_load_ps(m0.data00());
		__m256 v1 = _mm256_load_ps(m1.data00());
		__m256 v3 = _mm256_add_ps(v0, v1);
		_mm256_store_ps(mres.data00(), v3);

		v0 = _mm256_load_ps(m0.data20());
		v1 = _mm256_load_ps(m1.data20());
		v3 = _mm256_add_ps(v0, v1);
		_mm256_store_ps(mres.data20(), v3);
	}

	static inline void substraction(const DMatrixf& m0, const DMatrixf& m1, DMatrixf& mres) noexcept
	{
		__m256 v0 = _mm256_load_ps(m0.data00());
		__m256 v1 = _mm256_load_ps(m1.data00());
		__m256 v3 = _mm256_sub_ps(v0, v1);
		_mm256_store_ps(mres.data00(), v3);

		v0 = _mm256_load_ps(m0.data20());
		v1 = _mm256_load_ps(m1.data20());
		v3 = _mm256_sub_ps(v0, v1);
		_mm256_store_ps(mres.data20(), v3);
	}

	static inline void multiplication(const DMatrixf& m0, const DMatrixf& m1,  DMatrixf& mres)
	{
		const float* m0Data = m0.data00();
		const float* m1Data = m1.data00();
		float* mResData = mres.data00();

		for (size_t i = 0; i < 2; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				__m256 v0 = _mm256_load_ps(m0Data);
				__m256 v1 = _mm256_load_ps(m1Data);
				__m256 vres = _mm256_mul_ps(v0, v1);

				float r0Res = 0.0f;
				float r1Res = 0.0f;
				for (size_t k = 0; k < 4; ++k)
				{
					r0Res += vres.m256_f32[k];
					r1Res += vres.m256_f32[k + 4];
				}

				mResData[i * 8 + j] = r0Res;
				mResData[i * 8 + j + 4] = r1Res;
			}
		}
	}

	inline const float* data00() const
	{
		return &m00;
	}

	inline float* data00()
	{
		return &m00;
	}

	inline const float* data20() const
	{
		return &m20;
	}

	inline float* data20()
	{
		return &m20;
	}

	inline const float* data30() const
	{
		return &m30;
	}

	inline float* data30()
	{
		return &m30;
	}
};


WML_END


