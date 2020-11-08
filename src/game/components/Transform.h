#pragma once
#include "../../engine/ecs/Component.h"
#include "../utils/Utils.h"

struct Transform : public Component
{
	Transform(float x = 0.0f, float y = 0.0f, float r = 0.0f, float sx = 1.0f, float sy = 1.0f) : position({ x, y }), rotation(r), scale({ sx, sy }) {}

	vec2 position;
	float rotation;
	vec2 scale;
};