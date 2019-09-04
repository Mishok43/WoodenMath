#pragma once
#include "stdafx.h"
#include <limits>

WML_BEGIN

namespace FloatUtils
{
	static constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
	static constexpr float FLOAT_INF = std::numeric_limits<float>::infinity();
	static constexpr float FLOAT_MACHINE_EPSILON = std::numeric_limits<float>::epsilon()*0.5;

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


