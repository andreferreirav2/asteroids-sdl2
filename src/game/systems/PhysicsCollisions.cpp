#include "PhysicsCollisions.h"
#include "../components/CircleCollider.h"
#include "../components/Transform.h"
#include "iostream"


void PhysicsCollisions::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	// Clear all colliders
	for (auto& collider : manager.getAllComponentsOfType<CircleCollider>())
	{
		collider->collidingEntities.clear();
	}

	auto entities = manager.getAllEntitiesWithComponentType<CircleCollider>();
	for (auto it1 = entities.begin(); it1 != entities.end(); ++it1)
	{
		Entity e1 = *it1;
		auto const& collider1 = manager.getComponentOfType<CircleCollider>(e1);
		auto const& transform1 = manager.getComponentOfType<Transform>(e1);

		if (collider1 && transform1)
		{
			for (auto it2 = it1 + 1; it2 != entities.end(); ++it2)
			{
				Entity e2 = *it2;

				auto const& collider2 = manager.getComponentOfType<CircleCollider>(e2);
				auto const& transform2 = manager.getComponentOfType<Transform>(e2);

				if (collider2 && transform2 &&
					(collider1->layer & collider2->collidesWith
						|| collider2->layer & collider1->collidesWith))
				{
					float dist = sqrt(pow(transform1->position.x - transform2->position.x, 2) + pow(transform1->position.y - transform2->position.y, 2));
					if (dist < collider1->radius + collider2->radius)
					{
						collider1->collidingEntities.insert(e2);
						collider2->collidingEntities.insert(e1);
					}
				}
			}
		}
	}
}
