#pragma once
#include "../../engine/ecs/Component.h"

struct Weapon : public Component
{
	Weapon(float shotInterval = 0.0f) : shotInterval(shotInterval), timeToNextShot(0.0f), shooting(false) {};


	/* // TODO: pass time
		bool shoot()
	{
		if (timeToNextShot <= 0.0f)
		{
			shooting = true;
			timeToNextShot = shotInterval;
			return true;
		}
		return false;
	}
	*/

	float shotInterval;
	float timeToNextShot;
	bool shooting;
};