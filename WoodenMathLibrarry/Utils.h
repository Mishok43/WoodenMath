#pragma once
#include "stdafx.h"
#include "DVector.h"


WML_BEGIN

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

