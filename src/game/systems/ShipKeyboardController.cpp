#include "ShipKeyboardController.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include "../components/Engine.h"
#include "../components/Weapon.h"
#include "../components/SecondaryWeapon.h"
#include "../components/ShipManualControls.h"

#include <iostream>

void ShipKeyboardController::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<ShipManualControls>())
	{
		auto shipControls = manager.getComponentOfType<ShipManualControls>(e);

		auto engine = manager.getComponentOfType<Engine>(e);
		if (engine)
		{
			if (inputs->isPressed(shipControls->accelerate))
			{
				engine->thrustValue = 1.0f;
			}
			else
			{
				engine->thrustValue = 0.0f;
			}

			if (inputs->isPressed(shipControls->turnLeft))
			{
				engine->turnValue = 1.0f;
			}
			else if (inputs->isPressed(shipControls->turnRight))
			{
				engine->turnValue = -1.0f;
			}
			else
			{
				engine->turnValue = 0.0f;
			}
		}

		auto weapon = manager.getComponentOfType<Weapon>(e);
		if (weapon)
		{
			weapon->setTrigger(inputs->isPressed(shipControls->shootPrimary));
		}

		auto weapon2nd = manager.getComponentOfType<SecondaryWeapon>(e);
		if (weapon2nd)
		{
			weapon2nd->setTrigger(inputs->isPressed(shipControls->shootSecondary));
		}


		/*
		auto secondaryWeapon = manager.getComponentOfType<SecondaryWeapon>(e);
		if (secondaryWeapon)
		{
			secondaryWeapon->shooting = inputs->isPressed(shipControls->shoot);
		}
		*/
	}
}
