#pragma once
#include "stdafx.h"
#include "DVector.h"

WML_BEGIN

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

inline float radians(float degree)
{
	return 0.01745329252*degree;
}


inline float lerp(float a, float b, float t)
{
	return a * (1.0 - t) + b * t;
}

inline bool getRootsQuadraticEquation(float a, float b, float c, float& x0, float& x1)
{
	double discrim = (double)b * (double)b - 4 * (double)a * (double)c;
	if (discrim < 0) return false;
	double rootDiscrim = std::sqrt(discrim);

	double q;
	if (b < 0) q = -.5 * (b - rootDiscrim);
	else       q = -.5 * (b + rootDiscrim);
	x0 = q / a;
	x1 = c / q;
	if (x0 > x1) std::swap(x0, x1);
	return true;
}
inline bool solveLinearSystem2x2(const float A[2][2], const float B[2], float*x0,
								 float *x1)
{
	float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
	if (std::abs(det) < 1e-10f) return false;
	*x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
	*x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
	if (std::isnan(*x0) || std::isnan(*x1)) return false;
	return true;
}

inline float clamp(float t, float a, float b)
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

template <typename Predicate> 
inline int findInterval(int size, const Predicate &pred)
{
	int first = 0, len = size;
	while (len > 0)
	{
		int half = len >> 1, middle = first + half;
		
		if (pred(middle))
		{
			first = middle + 1;
			len -= half + 1;
		}
		else
		{
			len = half;
		}
	}
	return clamp(first - 1, 0, size - 2);
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
	return max(0.0f, 1.0f - cos2Theta(w));
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


inline DVector3f sphericalToCasterian(float sinTheta,
									float cosTheta,
									float phi)
{
	return DVector3f(sinTheta*std::cos(phi),
					 sinTheta*std::sin(phi),
					 cosTheta);
}

inline bool catmullRomWeights(int size, const float *nodes, float x,
					   int *offset, float *weights)
{

	if (!(x >= nodes[0] && x <= nodes[size - 1]))
		return false;

	
	int idx = findInterval(size, [&](int i)
	{
		return nodes[i] <= x;
	});
	*offset = idx - 1;
	float x0 = nodes[idx], x1 = nodes[idx + 1];
	float t = (x - x0) / (x1 - x0), t2 = t * t, t3 = t2 * t;

	
	weights[1] = 2 * t3 - 3 * t2 + 1;
	weights[2] = -2 * t3 + 3 * t2;

	
	if (idx > 0)
	{
		float w0 = (t3 - 2 * t2 + t) * (x1 - x0) / (x1 - nodes[idx - 1]);
		weights[0] = -w0;
		weights[2] += w0;
	}
	else
	{
		float w0 = t3 - 2 * t2 + t;
		weights[0] = 0;
		weights[1] -= w0;
		weights[2] += w0;
	}
	
	if (idx + 2 < size)
	{
		float w3 = (t3 - t2) * (x1 - x0) / (nodes[idx + 2] - x0);
		weights[1] -= w3;
		weights[3] = w3;
	}
	else
	{
		float w3 = t3 - t2;
		weights[1] -= w3;
		weights[2] += w3;
		weights[3] = 0;
	}

	return true;
}

inline DVector3f sphericalToCasterian(float sinTheta, float cosTheta, float phi,
										  const DVector3f& x, const DVector3f& y, const DVector3f& z)
{
	DVector3f basis = sphericalToCasterian(sinTheta, cosTheta, phi);
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

