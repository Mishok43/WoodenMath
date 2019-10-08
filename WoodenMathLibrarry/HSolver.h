#pragma  once
#include "stdafx.h"

WML_BEGIN

namespace Solvers{
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
}
WML_END