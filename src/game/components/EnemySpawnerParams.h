#pragma once
#include "../../engine/ecs/Component.h"

struct EnemySpawnerParams : public Component
{
	EnemySpawnerParams(int perMinute,
		int marginFromEdge,
		int enemyColliderLayer,
		int enemyCollidesWith,
		int enemyWeaponColliderLayer,
		int enemyWeaponCollidesWith)
		:
		perMinute(perMinute),
		marginFromEdge(marginFromEdge),
		enemyColliderLayer(enemyColliderLayer),
		enemyCollidesWith(enemyCollidesWith),
		enemyWeaponColliderLayer(enemyWeaponColliderLayer),
		enemyWeaponCollidesWith(enemyWeaponCollidesWith),
		timeToNextSpawn(20.0f)
	{
	}

	int perMinute;
	int marginFromEdge;

	int enemyColliderLayer;
	int enemyCollidesWith;
	int enemyWeaponColliderLayer;
	int enemyWeaponCollidesWith;

	float timeToNextSpawn;
};
