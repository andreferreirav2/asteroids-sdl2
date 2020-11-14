#pragma once
#include "../../engine/ecs/Component.h"

// Attach to Entities to destroy them after x seconds
struct DestroyAfterTime : public Component
{
	DestroyAfterTime(float ticks) : ticks(ticks) {}

	float ticks;
};