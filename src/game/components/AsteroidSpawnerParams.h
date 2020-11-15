#pragma once
#include "../../engine/ecs/Component.h"

struct AsteroidSpawnerParams : public Component
{
	AsteroidSpawnerParams(float minInterval,
		float maxInterval,
		int marginFromEdge,
		float smallRatio,
		float mediumRatio,
		float bigRatio,
		int asteroidsColliderLayer,
		int asteroidsCollidesWith)
		:
		minInterval(minInterval),
		maxInterval(maxInterval),
		marginFromEdge(marginFromEdge),
		smallRatio(smallRatio),
		mediumRatio(mediumRatio),
		bigRatio(bigRatio),
		asteroidsColliderLayer(asteroidsColliderLayer),
		asteroidsCollidesWith(asteroidsCollidesWith),
		timeToNextSpawn(0.0f)
	{
	}

	float minInterval;
	float maxInterval;

	int marginFromEdge;

	float smallRatio;
	float mediumRatio;
	float bigRatio;

	int asteroidsColliderLayer;
	int asteroidsCollidesWith;

	float timeToNextSpawn;
};
