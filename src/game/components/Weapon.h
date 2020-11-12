#pragma once
#include "../../engine/ecs/Component.h"
#include "../components/Transform.h"
#include <functional>
#include <memory>

struct Weapon : public Component
{
	Weapon(float shotInterval, std::function<void(std::shared_ptr<Transform>)> weaponSpawn) : shotInterval(shotInterval), weaponSpawn(weaponSpawn), timeToNextShot(0.0f), shooting(false) {};


	bool setTrigger(bool isPulled)
	{
		if (isPulled && timeToNextShot <= 0.0f)
		{
			shooting = true;
			timeToNextShot = shotInterval;
			return true;
		}

		shooting = false;
		return false;
	}

	float shotInterval;
	float timeToNextShot;
	bool shooting;
	std::function<void(std::shared_ptr<Transform>)> weaponSpawn;
};