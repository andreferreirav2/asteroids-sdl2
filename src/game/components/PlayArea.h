#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/Types.h"

struct PlayArea : public Component
{
	PlayArea(rect area) : area(area) {}

	rect area;
};