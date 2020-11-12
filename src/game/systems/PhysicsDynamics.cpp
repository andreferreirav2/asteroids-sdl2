#include "PhysicsDynamics.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include <algorithm>
#include <math.h>
#include "iostream"

#define PI 3.14159265f
#define EPSILON 0.001f

void PhysicsDynamics::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<RigidBody>())
	{
		auto rb = manager.getComponentOfType<RigidBody>(e);
		auto transform = manager.getComponentOfType<Transform>(e);

		if (rb && transform)
		{
			float velMagnitude = sqrt(pow(rb->velocity.x, 2) + pow(rb->velocity.y, 2));
			if (velMagnitude != 0.0f)
			{
				rb->velocity.x += rb->drag * dt * -rb->velocity.x;
				rb->velocity.y += rb->drag * dt * -rb->velocity.y;
			}

			if (std::abs(rb->velocity.x) < EPSILON)
			{
				rb->velocity.x = 0.0f;
			}
			if (std::abs(rb->velocity.y) < EPSILON)
			{
				rb->velocity.y = 0.0f;
			}

			transform->position.x += rb->velocity.x * dt;
			transform->position.y += rb->velocity.y * dt;
		}

	}
}
