#include "WeaponFiring.h"
#include "../../components/Weapon.h"
#include "../../components/SecondaryWeapon.h"
#include "../../components/Transform.h"
#include "../../components/RigidBody.h"
#include "../../components/Clock.h"
#include "../../components/SpriteSDL.h"
#include "../../components/CircleCollider.h"
#include <cmath>
#include <memory>
#include <string>
#include "../../../engine/Math.h"

void WeaponFiring::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	for (Entity e : manager.getAllEntitiesWithComponentType<Weapon>())
	{
		auto weapon = manager.getComponentOfType<Weapon>(e);

		if (weapon->shooting)
		{
			auto transform = manager.getComponentOfType<Transform>(e);
			auto rb = manager.getComponentOfType<RigidBody>(e);
			if (transform && rb)
			{
				weapon->weaponSpawn(transform, rb);
			}
		}

		weapon->onUpdate(dt);
	}

	for (Entity e : manager.getAllEntitiesWithComponentType<SecondaryWeapon>())
	{
		auto secondaryWeapon = manager.getComponentOfType<SecondaryWeapon>(e);

		if (secondaryWeapon->shooting)
		{
			auto transform = manager.getComponentOfType<Transform>(e);
			auto rb = manager.getComponentOfType<RigidBody>(e);
			if (transform && rb)
			{
				secondaryWeapon->weaponSpawn(transform, rb);
			}
		}
		secondaryWeapon->onUpdate(dt);
	}
}
