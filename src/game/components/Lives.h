#pragma once
#include "../../engine/ecs/Component.h"

struct Lives : public Component
{
	Lives(int left = 1) : left(left) {}

	int left;
};