#pragma once
#include "../../engine/ecs/Component.h"

struct ScoreAwarder : public Component
{
	ScoreAwarder(int score) : score(score) {}
	int score;
};