#pragma once

#include "stdafx.h"
#include "DMatrix.h"
#include "DQuaternion.h"

WML_BEGIN


template<typename T, uint8_t alignment = sse_alignment_size_v<__m256_t<T>>>
class alignas(alignment) DTransform
{
public:

	DTransform(DMatrix<T> m, DMatrix<T> mInv) :
		m(std::move(m)), mInv(std::move(mInv))
	{}

	inline DTransform operator*(const DTransform& v) const
	{
		return DTransform(m*v.m, mInv*v.mInv);
	}

	inline DTransform& operator*=(const DTransform& v)
	{
		m *= v.m;
		mInv *= v.mInv;
		return *this;
	}

	template<typename lT>
	inline DVector<lT, 3> operator()(const DVector<lT, 3>& v) const
	{
		return v * mData;
	}

	template<typename lT>
	inline DVector<lT, 4> operator()(const DVector<lT, 4>& v) const
	{
		return v * mData;
	}

	void inverse()
	{
		std::swap(mData, mInvData);
	}

	void transpose()
	{
		mData.transpose();
		mInvData.transpose();
	}

	const DMatrix<T>& m() const
	{
		return mData;
	}

	const DMatrix<T>& mInv() const
	{
		return mInvData;
	}

public:
	static DTransform makeTranslate(T x, T y, T z)
	{
		DTransform t;
		t.mData.setTransition(x, y, z);
		t.mInvData.setTransition(-x, -y, -y);
		return t;
	}

	static DTransform makeTranslate(const DVector<T, 3>& v)
	{
		DTransform t;
		t.mData.setTransition(v);
		t.mInvData.setTransition(-v);
		return t;
	}

	static DTransform makeScale(T x, T y, T z)
	{
		DTransform t;
		t.mData.setScale(x, y, z);
		t.mInvData.setScale(DVector<T, 3>(1.0) / DVector<T, 3>(x, y, z));
		return t;
	}

	static DTransform makeScale(const DVector<T, 3>& v)
	{
		DTransform t;
		t.mData.setScale(v);
		t.mInvData.setScale(DVector<T,3>(1.0)/v);
		return t;
	}

	static DTransform makeRotateX(T angle)
	{
		DTransform t;
		T cosAngle = cos(angle);
		T sinAngle = sin(angle);

		t.mData = DMatrix<T>(
			1.0, 0.0, 0.0, 0.0,
			0.0, cosAngle, -sinAngle, 0.0,
			0.0, sinAngle, cosAngle, 0.0,
			0.0, 0.0, 0.0, 1.0);

		t.mInvData = DMatrix<T>.transpose(t.mData);
	}

	static DTransform makeRotateY(T angle)
	{
		DTransform t;
		T cosAngle = cos(angle);
		T sinAngle = sin(angle);

		t.mData = DMatrix<T>(
			cosAngle, 0.0, sinAngle,  0.0,
			0.0,	  1.0, 0.0,		  0.0,
			-sinAngle, 0.0, cosAngle, 0.0,
			0.0, 0.0, 0.0, 1.0);

		t.mInvData = DMatrix<T>.transpose(t.mData);
	}

	static DTransform makeRotateZ(T angle)
	{
		DTransform t;
		T cosAngle = cos(angle);
		T sinAngle = sin(angle);

		t.mData = DMatrix<T>(
			cosAngle, -sinAngle, 0.0, 0.0,
			sinAngle, cosAngle, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);

		t.mInvData = DMatrix<T>.transpose(t.mData);
	}

	static DTransform makeRotate(DVector<T, 3> axis, T angle)
	{
		DQuaternion<T> q(std::move(axis), angle);
		DTransform t;
		t.mData = DQuaternion<T>.makeMatrix(q);
		t.mInvData = DMatrix<T>.transpose(t.mData);
		return t;
	}

	static DTransform makeLookAt(const DVector<T, 3>& pos, const DVector<T, 3>& look, const DVector<T, 3>& up)
	{
		DTransform t;
		DVector<T, 3> zBasis = DVector<T, 3>::normalize(look - pos);
		DVector<T, 3> xBasis = DVector<T, 3>::normalize(DVector<T, 3>::cross(up, zBasis));
		DVector<T, 3> yBasis = DVector<T, 3>::cross(zBasis, xBasis);

		t.mInvData = DMatrix<T>(
			xBasis.x(), xBasis.y(), xBasis.z(), 0.0,
			yBasis.z(), yBasis.y(), yBasis.z(), 0.0,
			zBasis.x(), zBasis.y(), zBasis.z(), 0.0,
			pos.x(), pos.y(), pos.z()
		);

		t.mData = DMatrix<T>(
			xBasis.x(), yBasis.x(), zBasis.x(), 0.0,
			xBasis.y(), yBasis.y(), zBasis.y(), 0.0,
			xBasis.z(), yBasis.z(), zBasis.z(), 0.0,
			-pos.x(), -pos.y(), -pos.z()
		);
		return t;
	}
protected:
	DMatrix<T> mData;
	DMatrix<T> mInvData;
};

WML_END

