#pragma once
#include "../../engine/ecs/Component.h"

struct Score : public Component
{
	Score(int score = 0) : score(score) {};
	int score = 0;
};