#pragma once
#include "../../engine/ecs/Component.h"

struct EnemySpawnerParams : public Component
{
	EnemySpawnerParams(float minInterval,
		float maxInterval,
		int marginFromEdge,
		int enemyColliderLayer,
		int enemyCollidesWith,
		int enemyWeaponColliderLayer,
		int enemyWeaponCollidesWith)
		:
		minInterval(minInterval),
		maxInterval(maxInterval),
		marginFromEdge(marginFromEdge),
		enemyColliderLayer(enemyColliderLayer),
		enemyCollidesWith(enemyCollidesWith),
		enemyWeaponColliderLayer(enemyWeaponColliderLayer),
		enemyWeaponCollidesWith(enemyWeaponCollidesWith),
		timeToNextSpawn(20.0f)
	{
	}

	float minInterval;
	float maxInterval;

	int marginFromEdge;

	int enemyColliderLayer;
	int enemyCollidesWith;
	int enemyWeaponColliderLayer;
	int enemyWeaponCollidesWith;

	float timeToNextSpawn;
};
