#include "WeaponFiring.h"
#include "../components/Weapon.h"
#include "../components/SecondaryWeapon.h"
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
	for (Entity e : manager.getAllEntitiesWithComponentType<Weapon, Transform, RigidBody>())
	{
		auto weapon = manager.getComponentOfType<Weapon>(e);

		if (weapon->shooting)
		{
			weapon->weaponSpawn(manager.getComponentOfType<Transform>(e), manager.getComponentOfType<RigidBody>(e));
		}
		weapon->timeToNextShot -= dt;
	}

	for (Entity e : manager.getAllEntitiesWithComponentType<SecondaryWeapon, Transform, RigidBody>())
	{
		auto secondaryWeapon = manager.getComponentOfType<SecondaryWeapon>(e);

		if (secondaryWeapon->shooting)
		{
			secondaryWeapon->weaponSpawn(manager.getComponentOfType<Transform>(e), manager.getComponentOfType<RigidBody>(e));
		}
		secondaryWeapon->timeToNextShot -= dt;
	}
}
