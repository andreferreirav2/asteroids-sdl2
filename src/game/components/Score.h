#pragma once
#include "../../engine/ecs/Component.h"

struct Score : public Component
{
	Score(int score) : score(score) {};
	int score = 0;
};