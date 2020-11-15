#pragma once
#include <cstdlib>
#include "Types.h"

#define PI 3.14159265f
#define EPSILON 0.001f
#define RAD_2_DEG 57.2958f
#define DEG_2_RAG 0.0174533f


inline float randBetween(float a, float b)
{
	return a + (b - a) * rand() / RAND_MAX;
}

inline float randSign()
{
	return static_cast<float>(rand()) / RAND_MAX < 0.5 ? -1.0f : 1.0f;
}

inline float distance(float2 a, float2 b)
{
	return sqrt(a.x * b.x + a.y + b.y);
}
