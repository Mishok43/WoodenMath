#pragma once

#include "stdafx.h"
#include "Utils.h"
#include <vector>

WML_BEGIN


template<typename AllocT>
class DDistributionbPieceWise1D
{
public:
	DDistributionbPieceWise1D(const float* f, int n):
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

	float pdf(uint32_t i)
	{
		return func[i] / funcInt;
	}

	float pdfDiscrete(uint32_t i)
	{
		return func[i] / (funcInt*size());
	}

	float sampleContinuous(float u, float& off) const
	{
		int offset = findInterval(cdf.size(), [&](int index)
		{
			return cdf[index] <= u;
		});

		off = offset;

		float du = u - cdf[offset];
		if ((cdf[offset + 1] - cdf[offset]) > 0){
			du /= (cdf[offset + 1] - cdf[offset]);
		}
		
		return (offset + du) / size();
	}

	int sampleDiscrete(float u, float& off, float& tQuantile) const
	{
		int offset = findInterval(cdf.size(),
								  [&](int index)
		{
			return cdf[index] <= u;
		});

		tQuantile = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
		return offset;
	}


protected:
	std::vector<float, AllocT<float>> func, cdf;
	float funcInt;
};

WML_END

