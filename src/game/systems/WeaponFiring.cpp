#include "WeaponFiring.h"
#include "../components/Weapon.h"
#include "../components/Transform.h"
#include "../components/RigidBody.h"
#include "../components/Clock.h"
#include "../components/SpriteSDL.h"
#include "../components/CircleCollider.h"
#include <cmath>
#include <memory>
#include <string>


#define RAD_2_DEG 57.2958f
#define DEG_2_RAG 0.0174533f

void WeaponFiring::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	for (Entity e : manager.getAllEntitiesWithComponentType<Weapon>())
	{
		auto weapon = manager.getComponentOfType<Weapon>(e);
		if (weapon)
		{
			auto transform = manager.getComponentOfType<Transform>(e);
			if (transform && weapon->shooting)
			{
				weapon->weaponSpawn(transform);
			}
			weapon->timeToNextShot -= dt;
		}
	}
}
