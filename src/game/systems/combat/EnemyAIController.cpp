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


void EnemyAIController::onStart(ECSManager& manager)
{

}

void EnemyAIController::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<ShipAIControls>())
	{
		auto ai = manager.getComponentOfType<ShipAIControls>(e);
		auto transform = manager.getComponentOfType<Transform>(e);
		auto rb = manager.getComponentOfType<RigidBody>(e);
		auto engine = manager.getComponentOfType<Engine>(e);
		auto weapon = manager.getComponentOfType<Weapon>(e);
		if (ai && transform && rb && engine && weapon)
		{
			// Fetch position of closest player
			float2 closestPlayerPos;
			float distToClosestPlayer;
			bool foundPlayer = false;
			for (Entity player : manager.getAllEntitiesWithComponentType<Player>())
			{
				auto playerTransform = manager.getComponentOfType<Transform>(player);
				if (playerTransform)
				{
					auto distToPlayer = distance(transform->position, playerTransform->position);
					if (!foundPlayer || distToPlayer < distToClosestPlayer)
					{
						distToClosestPlayer = distToPlayer;
						closestPlayerPos = playerTransform->position;
						foundPlayer = true;
					}
				}
			}

			// validate current AI state, chose new state, trigger state side-effects
			bool shooting = false;
			bool engineThrustValue = 0.0f;
			bool engineTurnValue = 0.0f;

			if (foundPlayer)
			{
				if (weapon->timeToNextShot <= 0) // AIM + FIRE
				{
					if (true) // aiming at ship
					{
						shooting = true;
					}
					else // not aiming, aim better
					{

					}
				}
				else // FLEE
				{
					float2 furthestFromPlayer = {};
				}
			}

			weapon->setTrigger(shooting);
			engine->thrustValue = engineThrustValue;
			engine->turnValue = engineTurnValue;
		}
	}
}
