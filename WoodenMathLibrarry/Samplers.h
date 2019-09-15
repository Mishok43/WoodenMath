#pragma once
#include "stdafx.h"
#include "DPoint.h"
#include <random>
#include <algorithm>

WML_BEGIN

DPoint2f sampleDistRejectionUniform()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> u(0.0, 1.0);

	DPoint2f p;
	do
	{
		p = DPoint2f(u(gen), u(gen));
		p *= 2.0;
		p -= DPoint2f(1.0, 1.0);
	} while (dot(p, p) > 1);

	return p;
}

DVector3f sampleHemisphereUniform(const DPoint2f& u)
{
	float z = u[0];
	float r = std::sqrt(std::max(0.0f, 1.0f - z * z));
	float phi = 2 * PI* u[1];
	return DVector3f(r * std::cos(phi), r * std::sin(phi), z);
}

float uniformHemispherePDF()
{
	return 1.0 / (2 * PI);
}

DVector3f sampleSphereUniform(const DPoint2f &u)
{
	float z = 1 - 2 * u[0];
	float r = std::sqrt(std::max((float)0, (float)1 - z * z));
	float phi = 2 * PI * u[1];
	return DVector3f(r * std::cos(phi), r * std::sin(phi), z);
}

DPoint2f sampleDiskUniform(const DPoint2f &u)
{
	float r = std::sqrt(u[0]);
	float theta = 2 * PI * u[1];
	return (DPoint2f(std::cos(theta), std::sin(theta))*r);
}

float hemisphereCosSinPDF(const float cosTheta)
{
	return cosTheta / PI;
}

DVector3f sampleHemisphereCosSin(const DPoint2f& u)
{
	DPoint2f d = sampleDiskUniform(u);
	float z = std::sqrt(std::max(0.0f, 1.0f - dot(d)));
	return DVector3f(d.x(), d.y(), z);
}

float coneUniformPDF(float cosThetaMax)
{
	return 1 / (2 * PI*(1 - cosThetaMax));
}
DVector3f UniformSampleCone(const DPoint2f &u, float cosThetaMax)
{
	float cosTheta = ((float)1 - u[0]) + u[0] * cosThetaMax;
	float sinTheta = std::sqrt((float)1 - cosTheta * cosTheta);
	float phi = u[1] * 2 * PI;
	return DVector3f(std::cos(phi) * sinTheta, std::sin(phi) * sinTheta,
					cosTheta);
}

DPoint2f sampleTriangleUniform(const DPoint2f &u)
{
	float su0 = std::sqrt(u[0]);
	return DPoint2f(1 - su0, u[1] * su0);
}

inline float PowerHeuristic(int nf, float fPdf, int ng, float gPdf)
{
	float f = nf * fPdf, g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}

WML_END

