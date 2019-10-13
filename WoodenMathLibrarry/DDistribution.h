#pragma once

#include "stdafx.h"
#include "Utils.h"
#include <vector>

WML_BEGIN


template<typename T>
class DDistributionbPieceWise1D
{
public:
	DDistributionbPieceWise1D()
	{
	}

	DDistributionbPieceWise1D(const T* f, int n):
		func(f, f+n), cdf(n+1)
	{
		cdf[0] = 0;
		for (int i = 1; i < n + 1; i++)
		{
			cdf[i] = cdf[i - 1] + func[i - 1] / n;
		}

		funcInt = cdf[n];

		for (int i = 1; i < n + 1; i++)
		{
			cdf[i] /= funcInt;
		}
	}
	
	uint32_t size() const
	{
		return func.size();
	}

	T pdf(uint32_t i)
	{
		return func[i] / funcInt;
	}

	T pdfDiscrete(uint32_t i)
	{
		return func[i] / (funcInt*size());
	}

	T sampleContinuous(T u, int& off, T& pdf) const
	{
		int offset = findInterval(cdf.size(), [&](int index)
		{
			return cdf[index] <= u;
		});

		off = offset;

		T du = u - cdf[offset];
		if ((cdf[offset + 1] - cdf[offset]) > 0){
			du /= (cdf[offset + 1] - cdf[offset]);
		}

		pdf = func[offset]/funcInt;
		
		return (offset + du) / size();
	}

	int sampleDiscrete(T u, T& tQuantile) const
	{
		int offset = findInterval(cdf.size(),
								  [&](int index)
		{
			return cdf[index] <= u;
		});

		tQuantile = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
		return offset;
	}


	T funcInt;
	std::vector<T> func, cdf;
};

template<typename T>
class DDistributionbPieceWise2D
{
public:
	DDistributionbPieceWise2D(){ }


	DDistributionbPieceWise2D(float* func, int nu, int nv)
	{
		conditionalV.resize(nv);
		for (int v = 0; v < nv; ++v)
		{
			conditionalV[v] = DDistributionbPieceWise1D(&func[nu*v], nu);
		}


		std::vector<float> marginalF;
		marginalF.resize(nv);
		for (int v = 0; v < nv; ++v)
		{
			marginalF[v] = conditionalV[v].funcInt;
		}

		marginalV = DDistributionbPieceWise1D(marginalF.data(), nv);
	}


	DPoint2f sampleContinuous(const DPoint2f &u, float &pdf) const
	{
		float pdfs[2];
		int v;
		int uo;
		float d1 = marginalV.sampleContinuous(u[1], v, pdfs[1]);
		float d0 = conditionalV[v].sampleContinuous(u[0], uo, pdfs[0]);
		pdf = pdfs[0] * pdfs[1];
		return DPoint2f(d0, d1);
	}

	float pdf(const DPoint2f &p) const
	{
		int iu = clamp(int(p[0] * conditionalV[0].size()),
					   0, conditionalV.size() - 1);
		int iv = clamp(int(p[1] * marginalV.size()),
					   0, marginalV.size() - 1);
		return conditionalV[iv].func[iu] / marginalV.funcInt;
	}

protected:
	std::vector<DDistributionbPieceWise1D<T>> conditionalV;
	DDistributionbPieceWise1D<T> marginalV;
};

WML_END

