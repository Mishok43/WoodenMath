#pragma once
#include "DVector.h"
#include "DMatrix.h"


WML_BEGIN

template<typename T, typename __mT = __m_t<T>, uint8_t alignment = sse_alignment_size_v<__mT>>
class alignas(alignment) DQuaternion: public DVector<T, 4>
{
public:
	DQuaternion(DVector<T, 3> u, T angle):
		DVector<T, 4>(u*sin(angle*0.5), cos(angle*0.5))
	{}

	DQuaternion() : 
		DVector<T, 4>(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	DQuaternion(T x, T y, T z, T angle) :
		DVector<T, 4>(DVector<T, 3>(x, y, z)*sin(angle*0.5), cos(angle*0.5))
	{}


	explicit DQuaternion(const DVector<T, 4>& vec) noexcept
		: DVector<T, 4>(vec.xmm)
	{ }

	explicit DQuaternion(DVector<T, 4>&& vec) noexcept
		: DVector<T, 4>(std::move(vec.xmm))
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

	inline DVector<T, 4>& operator*(const DVector<T, 4>& p) const
	{
		
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

	static inline T dot(const DQuaternion& q0, const DQuaternion& q1)
	{
		DVector<T, 4> wV = static_cast<DVector<T, 4>>(q0)*static_cast<DVector<T, 4>>(q1);
		return wV.w() + wV.x() + wV.y() + wV.z();
	}

	static inline T length(const DQuaternion& q)
	{
		return std::sqrt(dot(q, q));
	}

	static inline DQuaternion normalize(const DQuaternion& q)
	{
		return q / length(q);
	}

	static inline DMatrix<T> makeMatrix(const DQuaternion& q)
	{
		DVector<T, 4> q2t = static_cast<DVector<T, 4>>(q)*static_cast<DVector<T, 4>>(q);
		q2t = q2t.permute<0b10000001>(); // q2^2 q1^2 q1^2 q3^2 
		DVector<T, 4> q2d = q2t.permute<0b00001111>(); // q3^2 q3^2 q2^2 q2^2
		
		// 1.0-2.0*(q2^2-q3^2) | 1.0-2.0*(q1^2-q3^2) | 1.0-2.0*(q1^2-q2^2) 
		DVector<T, 4> res0 = DVector<T,4>(1.0)-(q2t + q2d)*2.0; 

		DVector<T, 4> sign = DVector<T, 4>(-1.0, 1.0, 1.0, -1.0);

		DVector<T, 4> q1q = DVector<T, 4>(q.x())*static_cast<DVector<T, 4>>(q);
		DVector<T, 4> q2q3 = DVector<T, 4>(q.y())*static_cast<DVector<T, 4>>(q);
		q2q3 = q2q3.permute<0b00001010>(); // q2q3 q2q3 

		DVector<T, 4> q4q = DVector<T, 4>(q.w())*static_cast<DVector<T, 4>>(q);
		DVector<T, 4> q4q1 = q4q.permute<0b00000000>(); //q4q1 q4q1

		q1q = q1q.permute<0b10011001>(); // q1q2 q1q3 q1q2 q1q3
		q4q = q4q.permute<0b01100110>(); // q4q3 q4q2 q4q3 q4q2

		// 2(q2q3 - q1q4) | 2(q2q3 + q1q4)
		DVector<T, 4> res1 = DVector<T, 4>::mAdd(q4q1, sign, q2q3)*2.0;
		// 2(q1q2 - q4q3) | 2(q1q3 + q4q2) | 2(q1q2 +q4q3) | 2(q1q3 - q4q2)
		DVector<T, 4> res2 = DVector<T, 4>::mAdd(q4q, sign, q1q)*2.0;
		
		return DMatrix<T>(
			res0[0], res2[2], res2[3], 0.0,
			res2[0], res0[1], res1[1], 0.0,
			res2[1], res1[0], res0[2], 0.0,
			0.0, 0.0, 0.0, 1.0
			);
	}

	static inline DQuaternion slerp(const DQuaternion& q0, const DQuaternion& q1, T t)
	{
		assert(t >= 0.0 && t <= 1.0);
		T angle = acos(dot(q0, q1));
		return (sin((1.0 - t)*angle)*q0 + sin(t*angle)*q1) / sin(angle);
	}

protected:
	static inline void mul(const DQuaternion& q0, const DQuaternion& q1, DQuaternion& q2)
	{
		T w0 = q0.getRealPart();
		T w1 = q1.getRealPart();

		DVector<T, 4> r = DVector<T, 4>::cross(q0, q1);
		r = DVector<T, 4>::mAdd(q1, w0, r);
		r = DVector<T, 4>::mAdd(q0, w1, r);
		
		DVector<T, 4> wV = static_cast<DVector<T, 4>>(q0)*static_cast<DVector<T, 4>>(q1);
		T w = wV.w() - wV.x() + wV.y() + wV.z();

		r.insert(3, w);

		q2 = DQuaternion(r);
	}
};

using DQuaternionf = DQuaternion<float>;
using DQuaterniond = DQuaternion<double>;

WML_END
