#include "PhysicsDynamics.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include <algorithm>
#include <math.h>
#include "iostream"
#include "../components/Clock.h"
#include "../../engine/Math.h"


void PhysicsDynamics::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	for (Entity e : manager.getAllEntitiesWithComponentType<RigidBody>())
	{
		auto transform = manager.getComponentOfType<Transform>(e);
		if (transform)
		{
			auto rb = manager.getComponentOfType<RigidBody>(e);

			float velMagnitude = rb->velocity.x * rb->velocity.x + rb->velocity.y * rb->velocity.y;
			if (velMagnitude > EPSILON)
			{
				rb->velocity.x += rb->drag * dt * -rb->velocity.x;
				rb->velocity.y += rb->drag * dt * -rb->velocity.y;
			}
			else
			{
				rb->velocity.x = 0.0f;
				rb->velocity.y = 0.0f;
			}

			transform->position.x += rb->velocity.x * dt;
			transform->position.y += rb->velocity.y * dt;
		}
	}
}
