#pragma once
#include "stdafx.h"
#include "DMatrix.h"
#include "DQuaternion.h"

WML_BEGIN

template<typename T, uint8_t alignment = sse_alignment_size_v<__m_t<T>>>
class DAnimatedTransform
{
	using Vector = typename DVector<T, 3>;
	using Quaternion = typename DQuaternion<T>;
public:
	DAnimatedTransform(Vector transition0, Vector scale0, Quaternion quaternion0, 
					   Vector transition1, Vector scale1, Quaternion quaternion1)
	{
		transitions[0] = std::move(transition0); scales[0] = std::move(scale0); quaternions[0] = std::move(quaternion0);
		transitions[1] = std::move(transition1); scales[1] = std::move(scale1); quaternions[1] = std::move(quaternion1);
	}


	DTransform interpolate(float time) const
	{
		return DTransform(
			Vector::lerp(transitions[0], transitions[1], time), 
			Vector::lerp(scales[0], scales[1], time),
			Quaternion::slerp(quaternions[0], quaternions[1], time));
	}

	DTransform interpolateSafe(float time) const
	{
		Vector trans;
		Vector scale;
		Quaternion rotation;

		if (time <= 0.0)
		{
			trans = transitions[0];
			scale = scales[0];
			rotation = quaternions[0];
		}
		else if (time >= 1.0)
		{
			trans = transitions[1];
			scale = scales[1];
			rotation = quaternions[1];
		}
		else
		{
			trans = Vector::lerp(transitions[0], transitions[1], time);
			scale = Vector::lerp(scales[0], scales[1], time);
			rotation = Quaternion::slerp(quaternions[0], quaternions[1], time);
		}

		return DTransform(trans, scale, rotation);
	}

	
	template<typename T>
	inline T operator()(T&& v, float t) const
	{
		return (interpolateSafe(t))(v);
	}

	template<typename lT>
	DBounds<lT, 3> boundsWithoutRotation(const DBounds<lT, 3>& b) const
	{
		using Bounds = DBounds<lT, 3>;
		return Bounds((transform0())(b), (transform1())(b));
	}

	const Vector& getTrans0() const{ return transitions[0]; }
	Vector& getTrans0() { return transitions[0]; }

	const Vector& getTrans1() const{ return transitions[1]; }
	Vector& getTrans1() { return transitions[1]; }

	const Vector& getScale0() const{ return scales[0]; }
	Vector& getScale0() { return scales[0]; }

	const Vector& getScale1() const{ return scales[1]; }
	Vector& getScale1() { return scales[1]; }

	const Vector& getRotation0() const{ return quaternions[0]; }
	Vector& getRotation0() { return quaternions[0]; }

	const Vector& getRotation1() const{ return quaternions[1]; }
	Vector& getRotation1() { return quaternions[1]; }


	DTransform transform0() const	{ return DTransform(transitions[0], scales[0], quaternions[0]); }
	DTransform transform1() const { return DTransform(transitions[1], scales[1], quaternions[1]); }

protected:
	Vector transitions[2]; // LastElement = startTime. 12/16 bytes. alignement = 16. 
	Vector scales[2]; // LastElement = endTime. 12/16 bytes. alignement = 16 
	Quaternion quaternions[2]; //16/16 bytes. alignement = 16
};

WML_END


