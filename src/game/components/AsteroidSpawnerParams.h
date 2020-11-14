#pragma once
#include "../../engine/ecs/Component.h"

struct AsteroidSpawnerParams : public Component
{
	AsteroidSpawnerParams(float minInterval, float maxInterval, float smallRatio, float mediumRatio, float bigRatio, int asteroidsColliderLayer, int asteroidsCollidesWith, rect playArea)
		: minInterval(minInterval), maxInterval(maxInterval), smallRatio(smallRatio), mediumRatio(mediumRatio), bigRatio(bigRatio), asteroidsColliderLayer(asteroidsColliderLayer), asteroidsCollidesWith(asteroidsCollidesWith), playArea(playArea), timeToNextSpawn(0.0f)
	{
	}

	float minInterval;
	float maxInterval;

	float smallRatio;
	float mediumRatio;
	float bigRatio;

	int asteroidsColliderLayer;
	int asteroidsCollidesWith;

	rect playArea;

	float timeToNextSpawn;
};
