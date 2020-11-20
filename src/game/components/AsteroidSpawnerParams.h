#pragma once
#include "../../engine/ecs/Component.h"

struct AsteroidSpawnerParams : public Component
{
	AsteroidSpawnerParams(int perMinute,
		int marginFromEdge,
		float smallRatio,
		float mediumRatio,
		float bigRatio,
		int asteroidsColliderLayer,
		int asteroidsCollidesWith)
		:
		perMinute(perMinute),
		marginFromEdge(marginFromEdge),
		smallRatio(smallRatio),
		mediumRatio(mediumRatio),
		bigRatio(bigRatio),
		asteroidsColliderLayer(asteroidsColliderLayer),
		asteroidsCollidesWith(asteroidsCollidesWith),
		timeToNextSpawn(0.0f)
	{
	}

	int perMinute;

	int marginFromEdge;

	float smallRatio;
	float mediumRatio;
	float bigRatio;

	int asteroidsColliderLayer;
	int asteroidsCollidesWith;

	float timeToNextSpawn;
};
