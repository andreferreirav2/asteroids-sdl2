#include "EnemyAI.h"
#include "../components/ShipAIControls.h"
#include "../components/Transform.h"
#include "../components/RigidBody.h"
#include <SDL.h>
#include <iostream>
#include "../components/Engine.h"
#include "../components/Weapon.h"


void EnemyAI::onStart(ECSManager& manager)
{

}

void EnemyAI::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
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
			// validate current AI state, chose new state, trigger state side-effects
		}
	}
}
