#include "ShipKeyboardController.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include "../components/Engine.h"
#include "../components/ShipKeyboardControls.h"

#include <iostream>

void ShipKeyboardController::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<ShipKeyboardControls>())
	{
		auto shipControls = manager.getComponentOfType<ShipKeyboardControls>(e);
		auto engine = manager.getComponentOfType<Engine>(e);
		if (shipControls && engine)
		{
			if (inputs->isPressed(shipControls->accelerate))
			{
				engine->thrustValue = 1.0f;
				std::cout << "engine GO!" << std::endl;
			}
			else
			{
				engine->thrustValue = 0.0f;
			}

			if (inputs->isPressed(shipControls->turnLeft))
			{
				engine->turnValue = -1.0f;
			}
			else if (inputs->isPressed(shipControls->turnRight))
			{
				engine->turnValue = 1.0f;
			}
			else
			{
				engine->turnValue = 0.0f;
			}
		}

		/*
		auto primaryWeapon = manager.getComponentOfType<PrimaryWeapon>(e);
		if (primaryWeapon)
		{
			primaryWeapon->shooting = inputs->isPressed(shipControls->shoot);
		}

		auto secondaryWeapon = manager.getComponentOfType<SecondaryWeapon>(e);
		if (secondaryWeapon)
		{
			secondaryWeapon->shooting = inputs->isPressed(shipControls->shoot);
		}
		*/
	}
}