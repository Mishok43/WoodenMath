#pragma once
#include "stdafx.h"
#include "DVector.h"

WML_BEGIN
template<typename T>
float lerp(T a, T b, float t)
{
	return a * (1.0 - t) + b * t;
}


float clamp(float t, float a, float b)
{
	if (t <= a)
	{
		return a;
	}
	else if (t >= b)
	{
		return b;
	}
	else
	{
		return t;
	}
}


inline float cosTheta(const DVector3f& w)
{
	return w.z();
}

inline float cos2Theta(const DVector3f& w)
{
	return w.z()*w.z();
}

inline float absCosTheta(const DVector3f& w)
{
	return std::abs(w.z());
}

inline float sin2Theta(const DVector3f& w)
{
	return std::max(0.0f, 1.0f - cos2Theta(w));
}

inline float sinTheta(const DVector3f& w)
{
	return std::sqrt(sin2Theta(w));
}

inline float tanTheta(const DVector3f& w)
{
	return sinTheta(w) / cosTheta(w);
}

inline float tan2Theta(const DVector3f& w)
{
	return sin2Theta(w) / cos2Theta(w);
}

inline float cosPhi(const DVector3f& w)
{
	float sTheta = sinTheta(w);
	return (sTheta == 0.0) ? 1.0 : clamp(w.x() / sTheta, -1.0, 1.0);
}

inline float sinPhi(const DVector3f &w)
{
	float sTheta = sinTheta(w);
	return (sTheta == 0) ? 1.0 : clamp(w.y() / sTheta, -1, 1);
}

inline float cos2Phi(const DVector3f &w)
{
	return cosPhi(w) * cosPhi(w);
}
inline float sin2Phi(const DVector3f &w)
{
	return sinPhi(w) * sinPhi(w);
}


inline DVector3f fromSphericalToCasterian(float sinTheta,
									float cosTheta,
									float phi)
{
	return DVector3f(sinTheta*std::cos(phi),
					 sinTheta*std::sin(phi),
					 cosTheta);
}


inline DVector3f fromSphericalToCasterian(float sinTheta, float cosTheta, float phi,
										  const DVector3f& x, const DVector3f& y, const DVector3f& z)
{
	DVector3f basis = fromSphericalToCasterian(sinTheta, cosTheta, phi);
	return x * basis.x() + y * basis.y() + z * basis.z();
}

inline float sphericalTheta(const DVector3f& v)
{
	return std::acos(v.z());
}

inline float sphericalPhi(const DVector3f& v)
{
	float p = std::atan2(v.y(), v.x());
	return (p < 0) ? (p + 2 * PI) : p;
}




WML_END

