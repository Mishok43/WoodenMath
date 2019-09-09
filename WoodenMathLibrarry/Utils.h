#pragma once
#include "stdafx.h"

WML_BEGIN
float lerp(float a, float b, float t)
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
WML_END


