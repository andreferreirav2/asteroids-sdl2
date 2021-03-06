#pragma once
#include "../../engine/ecs/Component.h"
#include <functional>
#include <memory>

struct Transform;
struct RigidBody;

struct Weapon : public Component
{
	Weapon(float shotInterval, std::function<void(std::shared_ptr<Transform>, std::shared_ptr<RigidBody>)> weaponSpawn) :
		shotInterval(shotInterval),
		timeToNextShot(0.0f),
		shooting(false),
		weaponSpawn(weaponSpawn)
	{
	};


	void onUpdate(float dt) { timeToNextShot -= dt; }
	bool canFire() { return timeToNextShot <= 0.0f; }

	bool setTrigger(bool isPulled)
	{
		if (isPulled && canFire())
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
	std::function<void(std::shared_ptr<Transform>, std::shared_ptr<RigidBody>)> weaponSpawn;
};