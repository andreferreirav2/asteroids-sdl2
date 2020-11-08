#include "PhysicsDynamics.h"
#include "../components/RigidBody.h"
#include "../components/Transform.h"

void PhysicsDynamics::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<RigidBody>())
	{
		auto rb = manager.getComponentOfType<RigidBody>(e);
		auto transform = manager.getComponentOfType<Transform>(e);

		if (rb && transform)
		{
			// rb->velocity.y += -9.81f * rb->mass * dt; // simulate gravity
			transform->position.x += rb->velocity.x * dt;
			transform->position.y += rb->velocity.y * dt;
		}

	}
}
