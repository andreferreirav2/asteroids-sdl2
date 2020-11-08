#pragma once
#include "../../engine/ecs/Component.h"
#include "../utils/Utils.h"

struct Transform : public Component
{
	Transform() : position({ 0.0f, 0.0f }), rotation(0.0f), scale({ 1.0f, 1.0f }) {}
	Transform(float x, float y) : position({ x, y }), rotation(0.0f), scale({ 1.0f, 1.0f }) {}
	Transform(float x, float y, float r, float sx, float sy) : position({ x, y }), rotation(r), scale({ sx, sy }) {}

	vec2 position;
	float rotation;
	vec2 scale;
};