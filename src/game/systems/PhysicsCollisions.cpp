#include "PhysicsCollisions.h"
#include "../components/CircleCollider.h"
#include "../components/Transform.h"
#include "iostream"
#include <vector>
#include <tuple>


void PhysicsCollisions::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	// Clear all colliders
	/*
	for (auto& collider : manager.getAllComponentsOfType<CircleCollider>())
	{
		collider->collidingEntities.clear();
	}
	*/

	//auto es = manager.getAllEntitiesWithComponentTypes<CircleCollider, Transform>();

	auto entitiesComponentTuple = manager.getAllEntitiesWithComponentTypes<CircleCollider, Transform>();
	auto entities = std::vector<std::tuple<Entity, std::shared_ptr<CircleCollider>, std::shared_ptr<Transform>>>(entitiesComponentTuple.begin(), entitiesComponentTuple.end());
	for (int i = 0; i < entities.size(); ++i)
	{
		auto& it1 = entities[i];
		Entity e1 = std::get<0>(it1);
		auto const& collider1 = std::get<1>(it1);
		auto const& transform1 = std::get<2>(it1);

		for (int j = i + 1; j < entities.size(); ++j)
		{
			auto& it2 = entities[j];
			Entity e2 = std::get<0>(it2);
			auto const& collider2 = std::get<1>(it2);
			auto const& transform2 = std::get<2>(it2);

			if (collider1->layer & collider2->collidesWith
				|| collider2->layer & collider1->collidesWith)
			{
				float deltaX = transform1->position.x - transform2->position.x;
				float deltaY = transform1->position.y - transform2->position.y;
				float radii = collider1->radius + collider2->radius;

				float dist2 = deltaX * deltaX + deltaY * deltaY;
				float radii2 = radii * radii;
				if (dist2 < radii2)
				{
					std::cerr << e1 << " is touching " << e2 << std::endl;
					manager.destroyEntity(e1);
					manager.destroyEntity(e2);
					//collider1->collidingEntities.insert(e2);
					//collider2->collidingEntities.insert(e1);
				}
			}
		}
	}
}
