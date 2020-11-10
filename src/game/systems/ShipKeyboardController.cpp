#include "ShipKeyboardController.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include "../components/Engine.h"
#include "../components/Weapon.h"
#include "../components/ShipManualControls.h"

#include <iostream>


void ShipKeyboardController::onStart(ECSManager& manager)
{
}

void ShipKeyboardController::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<ShipManualControls>())
	{
		auto shipControls = manager.getComponentOfType<ShipManualControls>(e);
		auto engine = manager.getComponentOfType<Engine>(e);
		auto weapon = manager.getComponentOfType<Weapon>(e);
		if (shipControls && engine)
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

		if (shipControls && weapon)
		{
			// TODO mix with weapon system to update time bewteen shots
			weapon->shooting = inputs->isPressed(shipControls->shoot);
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
