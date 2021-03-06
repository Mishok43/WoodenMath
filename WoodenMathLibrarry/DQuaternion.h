#pragma once
#include "DVector.h"
#include "DMatrix.h"


WML_BEGIN

template<typename T>
class ALIGN_AS_VEC(T, 4) DQuaternion: public DVector<T, 4>
{
public:
	DQuaternion(DVector<T, 3> u, T angle):
		DVector<T, 4>(std::move(u*sin(angle*0.5)), cos(angle*0.5))
	{}
	 
	DQuaternion() : 
		DVector<T, 4>(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	DQuaternion(T x, T y, T z, T angle) :
		DVector<T, 4>(DVector<T, 3>(x, y, z)*sin(angle*0.5), cos(angle*0.5))
	{}


	explicit DQuaternion(const DVector<T, 4>& vec) noexcept
		: DVector<T, 4>(vec)
	{ }

	explicit DQuaternion(DVector<T, 4>&& vec) noexcept
		: DVector<T, 4>(std::move(vec))
	{}

	DQuaternion& operator=(const DVector<T, 4>& vec) = delete;
	~DQuaternion() = default;

	inline DQuaternion operator+(const DQuaternion& q) const
	{
		return *this + q;
	}

	inline DQuaternion& operator+=(const DQuaternion& q) const
	{
		*this += q;
		return *this;
	}

	inline DQuaternion operator*(const DQuaternion& q) const
	{
		DQuaternion qRes;
		mul(*this, q, qRes);
		return qRes;
	}

	inline DQuaternion& operator*(const DQuaternion& q)
	{
		mul(*this, q, *this);
		return (*this);
	}

	inline T getRealPart() const
	{
		return (*this)[3];
	}

	inline T& getRealPart()
	{
		return (*this)[3];
	}

	inline DVector<T, 3> getImaginaryPart() const
	{
		return DVector<T, 3>(*this);
	}


	inline T getAngle() const
	{
		return acos(getRealPart())/2.0;
	}

	inline DVector<T, 3> getRotateDir() const
	{
		DVector<T, 3> dir = DVector<T, 3>(*this);
		return dir / sin(acos(getRealPart()));
	}

	template<uint8_t VSize>
	friend inline DVector<T, VSize> operator*(const DVector<T, VSize>& v, const DQuaternion& q) noexcept
	{
		DVector<T, VSize> res;
		mul(q, v, res);
		res.insert(3, v[3]);

		return res;
	}

protected:
	static inline void mul(const DQuaternion& q0, const DQuaternion& q1, DQuaternion& q2)
	{
		T w0 = q0.getRealPart();	
		T w1 = q1.getRealPart();

		DVector<T, 4> r = cross(q0, q1);
		r = mad(q1, w0, r);
		r = mad(q0, w1, r);
		
		DVector<T, 4> wV = static_cast<DVector<T, 4>>(q0)*static_cast<DVector<T, 4>>(q1);
		T w = wV.w() - wV.x() + wV.y() + wV.z();

		r.insert(3, w);

		q2 = DQuaternion(r);
	}

	template<uint8_t VSize>
	static inline void mul(const DQuaternion& u, const DVector<T, VSize>& v, DVector<T, VSize>& res)
	{	
		T w = u.getRealPart();
		// 2w(uXv)
		DVector<T, 4> r3 = cross(u, v)*(2*w);
		// 2(u*v)u + 2w(uXv)
		DVector<T, 4> r2 = mad(u, 2.0f*dot<3>(u, v), r3);

		DVector<T, 4> u2 = (DVector<T, 4>)(u)*(DVector<T, 4>)(u);
		// (w^2-u*u)v + 2(u*v)u + 2w(uXv)
		res = mad(v, u2.w()-(u2.x()+u2.y()+u2.z()), r2);
	}
};


template<typename T>
inline T dot(const DQuaternion<T>& q0, const DQuaternion<T>& q1)
{
	DVector<T, 4> wV = static_cast<DVector<T, 4>>(q0)*static_cast<DVector<T, 4>>(q1);
	return wV.w() + wV.x() + wV.y() + wV.z();
}

template<typename T>
inline T length(const DQuaternion<T>& q)
{
	return std::sqrt(dot(q, q));
}

template<typename T>
inline DQuaternion<T> normalize(const DQuaternion<T>& q)
{
	return q / length(q);
}

template<typename T> 
inline DMatrix<T> makeMatrix(const DQuaternion<T>& q)
{
	DVector<T, 4> q2t = static_cast<DVector<T, 4>>(q)*static_cast<DVector<T, 4>>(q);
	q2t = q2t.permuteEach<0b10000001>(); // q2^2 q1^2 q1^2 q3^2 
	DVector<T, 4> q2d = q2t.permuteEach<0b00001111>(); // q3^2 q3^2 q2^2 q2^2

	// 1.0-2.0*(q2^2-q3^2) | 1.0-2.0*(q1^2-q3^2) | 1.0-2.0*(q1^2-q2^2) 
	DVector<T, 4> res0 = DVector<T, 4>(1.0) - (q2t + q2d)*2.0;

	DVector<T, 4> sign = DVector<T, 4>(-1.0, 1.0, 1.0, -1.0);

	DVector<T, 4> q1q = DVector<T, 4>(q.x())*static_cast<DVector<T, 4>>(q);
	DVector<T, 4> q2q3 = DVector<T, 4>(q.y())*static_cast<DVector<T, 4>>(q);
	q2q3 = q2q3.permuteEach<0b00001010>(); // q2q3 q2q3 

	DVector<T, 4> q4q = DVector<T, 4>(q.w())*static_cast<DVector<T, 4>>(q);
	DVector<T, 4> q4q1 = q4q.permuteEach<0b00000000>(); //q4q1 q4q1

	q1q = q1q.permuteEach<0b10011001>(); // q1q2 q1q3 q1q2 q1q3
	q4q = q4q.permuteEach<0b01100110>(); // q4q3 q4q2 q4q3 q4q2

	// 2(q2q3 - q1q4) | 2(q2q3 + q1q4)
	DVector<T, 4> res1 = mad(q4q1, sign, q2q3)*2.0;
	// 2(q1q2 - q4q3) | 2(q1q3 + q4q2) | 2(q1q2 +q4q3) | 2(q1q3 - q4q2)
	DVector<T, 4> res2 = mad(q4q, sign, q1q)*2.0;

	return DMatrix<T>(
		res0[0], res2[2], res2[3], 0.0,
		res2[0], res0[1], res1[1], 0.0,
		res2[1], res1[0], res0[2], 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

template<typename T> 
inline DQuaternion<T> slerp(const DQuaternion<T>& q0, const DQuaternion<T>& q1, T t)
{
	assert(t >= 0.0 && t <= 1.0);
	T angle = acos(dot(q0, q1));
	return (q0*(T)(sin((1.0 - t)*angle)) + q1 * sin(t*angle)) / sin(angle);
}


using DQuaternionf = DQuaternion<float>;
using DQuaterniond = DQuaternion<double>;

WML_END
