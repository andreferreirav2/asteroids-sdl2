#include "EnginesThrusters.h"
#include "../components/Engine.h"
#include "../components/Transform.h"
#include "../components/RigidBody.h"
#include <cmath>
#include "../../engine/Math.h"
#include "../components/Clock.h"

void EnginesThrusters::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	for (Entity e : manager.getAllEntitiesWithComponentType<Engine>())
	{
		auto engine = manager.getComponentOfType<Engine>(e);
		if (engine->thrustValue != 0 || engine->turnValue)
		{
			auto transform = manager.getComponentOfType<Transform>(e);
			auto rb = manager.getComponentOfType<RigidBody>(e);

			if (transform && rb)
			{
				float engForwardSpeed = engine->thrustSpeed * engine->thrustValue * dt / rb->mass;
				transform->rotation += engine->turnSpeed * engine->turnValue * dt;
				rb->velocity.x += engForwardSpeed * cos(transform->rotation * DEG_2_RAG);
				rb->velocity.y -= engForwardSpeed * sin(transform->rotation * DEG_2_RAG);
			}
		}
	}
}
