#pragma once
#include "stdafx.h"

WML_BEGIN

namespace FloatUtils
{
	inline uint32_t floatToBits(float f)
	{
		uint32_t ui;
		std::memcpy(&ui, &f, sizeof(float));
		return ui;
	}

	inline float bitsToFloat(uint32_t ui)
	{
		float f;
		std::memcpy(&f, &ui, sizeof(uint32_t));
		return f;
	}

	inline float nextFloatUp(float v)
	{
		// position inf
		if (std::isinf(v) && v > 0.)
		{
			return v;
		}

		if (v == -.0f)
		{
			v = .0f;
		}

		uint32_t bits = floatToBits(v);
		if (v >= 0)
		{
			++bits;
		}
		else
		{
			--bits;
		}

		return bitsToFloat(bits);
	}
}

WML_END


