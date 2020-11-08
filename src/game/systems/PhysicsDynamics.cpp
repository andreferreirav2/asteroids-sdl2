#include "PhysicsDynamics.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"
#include <algorithm>

float const EPSILON = 0.00001f;

void PhysicsDynamics::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<RigidBody>())
	{
		auto rb = manager.getComponentOfType<RigidBody>(e);
		auto transform = manager.getComponentOfType<Transform>(e);

		if (rb && transform)
		{
			float velMagnitude = sqrt(pow(rb->velocity.x, 2) + pow(rb->velocity.y, 2));
			float dragMultiplier = std::max(static_cast<float>(velMagnitude * pow(rb->drag, 2.0f)), 0.0f);
			if (velMagnitude != 0.0f)
			{
				rb->velocity.x += dragMultiplier * -rb->velocity.x / velMagnitude;
				rb->velocity.y += dragMultiplier * -rb->velocity.y / velMagnitude;
			}

			if (rb->velocity.x < EPSILON)
			{
				rb->velocity.x = 0.0f;
			}
			if (rb->velocity.y < EPSILON)
			{
				rb->velocity.y = 0.0f;
			}

			// rb->velocity.y += -9.81f * rb->mass * dt; // simulate gravity
			transform->position.x += rb->velocity.x * dt;
			transform->position.y += rb->velocity.y * dt;
		}

	}
}
