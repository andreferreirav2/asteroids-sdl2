#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/Types.h"

struct Respawn : public Component
{
	Respawn(float2 position, float rotation) : position(position), rotation(rotation) {}

	float2 position;
	float rotation;
};