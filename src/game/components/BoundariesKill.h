#pragma once
#include "../../engine/ecs/Component.h"

// Attach to Entities that are supposed to die after going out of bounds
struct BoundariesKill : public Component
{
	BoundariesKill(int margin = 100) : margin(margin) {}

	int const margin;
};