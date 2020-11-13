#include "ShipGameController.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include "../components/Engine.h"
#include "../components/ShipManualControls.h"

#include <iostream>
#include <algorithm>

#define JOYSTICK_MARGIN 0.3f 

void ShipGameController::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	if (inputs->controllerEnabled)
	{
		for (Entity e : manager.getAllEntitiesWithComponentType<ShipManualControls, Engine, Transform>())
		{
			auto shipControls = manager.getComponentOfType<ShipManualControls>(e);
			auto engine = manager.getComponentOfType<Engine>(e);
			auto transform = manager.getComponentOfType<Transform>(e);

			float magnitude = sqrt(pow(inputs->leftAxis.x, 2) + pow(inputs->leftAxis.y, 2));
			std::cerr << magnitude << std::endl;
			if (magnitude > JOYSTICK_MARGIN)
			{
				transform->rotation = atan2(-pow(inputs->leftAxis.y, 3), pow(inputs->leftAxis.x, 3)) * (180.0 / 3.1415f);
				engine->thrustValue = std::min(pow(magnitude, 3), 1.0f);
			}
		}
	}
}
