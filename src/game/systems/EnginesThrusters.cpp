#include "EnginesThrusters.h"
#include "../components/Engine.h"
#include "../components/Transform.h"
#include "../components/RigidBody.h"
#include <cmath>

void EnginesThrusters::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<Engine>())
	{
		auto engine = manager.getComponentOfType<Engine>(e);
		auto transform = manager.getComponentOfType<Transform>(e);
		auto rb = manager.getComponentOfType<RigidBody>(e);
		if (engine && transform && rb)
		{
			float engForwardSpeed = engine->thrustSpeed * engine->thrustValue * dt / rb->mass;
			transform->rotation += engine->turnSpeed * engine->turnValue;
			rb->velocity.x += engForwardSpeed * cos(transform->rotation);
			rb->velocity.y += engForwardSpeed * sin(transform->rotation);
		}
	}
}
