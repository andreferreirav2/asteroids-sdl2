#include "EnginesThrusters.h"
#include "../components/Engine.h"
#include "../components/Transform.h"
#include "../components/RigidBody.h"
#include <cmath>

#define RAD_2_DEG 57.2958f
#define DEG_2_RAG 0.0174533f

void EnginesThrusters::onStart(ECSManager& manager)
{
}

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
			rb->velocity.x += engForwardSpeed * cos(transform->rotation * DEG_2_RAG);
			rb->velocity.y -= engForwardSpeed * sin(transform->rotation * DEG_2_RAG);
		}
	}
}
