#include "EnemyAIController.h"
#include "../../../engine/Math.h"
#include "../../../engine/Types.h"
#include "../../components/ShipAIControls.h"
#include "../../components/Transform.h"
#include "../../components/RigidBody.h"
#include "../../components/Engine.h"
#include "../../components/Weapon.h"
#include "../../components/Player.h"
#include <SDL.h>
#include <iostream>
#include "../../components/PlayArea.h"
#include "../../components/Boundless.h"
#include "../../components/Clock.h"

float angleBetweenVectors(float2 v1, float2 v2)
{
	float dot = v1.x * v2.x + v1.y * v2.y;
	float det = v1.x * v2.y - v1.y * v2.x;
	return atan2(det, dot);
}

void searchForClosestFoe(ECSManager& manager, float2 const& pos, float const& rotation, rect const& playArea, bool boundless,
	float* distToClosestFoe, float* angleToAimAtFoe)
{
	float2 closestFoePos;
	bool foundFoe = false;
	for (Entity foe : manager.getAllEntitiesWithComponentType<Player>())
	{
		auto foeTransform = manager.getComponentOfType<Transform>(foe);
		if (foeTransform)
		{
			auto foePos = foeTransform->position;
			if (boundless)
			{
				// TODO: Transform foe's position to adjust for worldwrap
			}

			auto distToFoe = distance(pos, foePos);
			if (!foundFoe || distToFoe < *distToClosestFoe)
			{
				*distToClosestFoe = distToFoe;
				closestFoePos = foePos;
				foundFoe = true;
			}
		}
	}

	if (foundFoe)
	{
		float cartesianRotation = (rotation + 180) * DEG_2_RAG;
		*angleToAimAtFoe = angleBetweenVectors(
			{ pos.x - closestFoePos.x, -(pos.y - closestFoePos.y) },
			{ cos(cartesianRotation), sin(cartesianRotation) });
	}
}

void EnemyAIController::onStart(ECSManager& manager)
{

}

void EnemyAIController::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	rect playArea = manager.getAllComponentsOfType<PlayArea>().begin()->get()->area; // What if there is no play area?
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	for (Entity e : manager.getAllEntitiesWithComponentType<ShipAIControls>())
	{
		auto ai = manager.getComponentOfType<ShipAIControls>(e);
		if (ai)
		{
			ai->onUpdate(dt);

			auto transform = manager.getComponentOfType<Transform>(e);
			auto rb = manager.getComponentOfType<RigidBody>(e);
			auto engine = manager.getComponentOfType<Engine>(e);
			auto weapon = manager.getComponentOfType<Weapon>(e);
			auto boundless = manager.getComponentOfType<Boundless>(e);

			if (transform && rb && engine && weapon)
			{
				// Fetch position of closest player
				float distToClosestFoe = 0.0f;
				float angleToAimAtFoe = 0.0f;
				searchForClosestFoe(manager, transform->position, transform->rotation, playArea, boundless != nullptr, &distToClosestFoe, &angleToAimAtFoe);

				// Switch states
				if (ai->canChangeState())
				{
					ShipAIStates nextState = ai->state;

					switch (ai->state)
					{
					case ShipAIStates::FLEE:
						nextState = ShipAIStates::AIM;
						break;
					case ShipAIStates::AIM:
						if (abs(angleToAimAtFoe) < EPSILON && weapon->canFire())
						{
							nextState = ShipAIStates::SHOOT;
						}
						break;
					case ShipAIStates::SHOOT:
						nextState = ShipAIStates::REST;
						break;
					case ShipAIStates::REST:
						nextState = ShipAIStates::FLEE;
						break;
						// TODO: Handle Chase
					}

					ai->switchState(nextState);
				}

				bool shooting = false;
				float engineThrustValue = 0.0f;
				float engineTurnValue = 0.0f;

				// State's actions
				switch (ai->state)
				{
				case ShipAIStates::FLEE:
					if (angleToAimAtFoe < 0)
					{
						engineTurnValue = -1.0f; // Turn left
					}
					else
					{
						engineTurnValue = 1.0f; // Turn right 
					}
					engineThrustValue = 1.0f;
					break;
				case ShipAIStates::AIM:
					if (abs(angleToAimAtFoe) < EPSILON)
					{
						engineTurnValue = 0.0f; // Stay still
					}
					else if (angleToAimAtFoe < 0)
					{
						engineTurnValue = 1.0f; // Turn right 
					}
					else
					{
						engineTurnValue = -1.0f; // Turn left
					}
					break;
				case ShipAIStates::SHOOT:
					shooting = true;
					break;
				case ShipAIStates::REST:
					break;
				}

				weapon->setTrigger(shooting);
				engine->thrustValue = engineThrustValue;
				engine->turnValue = engineTurnValue;
			}
		}
	}
}
