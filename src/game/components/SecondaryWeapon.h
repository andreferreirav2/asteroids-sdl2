#pragma once
#include "Weapon.h"
#include <functional>
#include <memory>

struct Transform;

struct SecondaryWeapon : public Weapon
{
	SecondaryWeapon(float shotInterval, unsigned int ammo, std::function<void(std::shared_ptr<Transform>, std::shared_ptr<RigidBody>)> weaponSpawn) : Weapon(shotInterval, weaponSpawn), ammo(ammo) {};

	unsigned int ammo;
};