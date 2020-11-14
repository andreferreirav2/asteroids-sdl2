#pragma once
#include "../../engine/ecs/Component.h"

struct Score : public Component
{
	int score = 0;
};