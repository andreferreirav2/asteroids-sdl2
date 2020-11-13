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
	size_t entitiesSize = entities.size();
	for (int i = 0; i < entitiesSize; ++i)
	{
		auto& it1 = entities[i];
		Entity e1 = std::get<0>(it1);
		auto const& collider1 = std::get<1>(it1);
		auto const& transform1 = std::get<2>(it1);
		auto const& transform1Position = transform1->position;
		auto const& collider1Radius = collider1->radius;
		auto const& collider1Layer = collider1->layer;
		auto const& collider1CollidesWith = collider1->collidesWith;

		for (int j = i + 1; j < entitiesSize; ++j)
		{
			auto& it2 = entities[j];
			Entity e2 = std::get<0>(it2);
			auto const& collider2 = std::get<1>(it2);
			auto const& transform2 = std::get<2>(it2);
			auto const& transform2Position = transform2->position;
			auto const& collider2Radius = collider2->radius;
			auto const& collider2Layer = collider2->layer;
			auto const& collider2CollidesWith = collider2->collidesWith;

			if (collider1Layer & collider2CollidesWith || collider2Layer & collider1CollidesWith)
			{
				float deltaX = transform1Position.x - transform2Position.x;
				float deltaY = transform1Position.y - transform2Position.y;
				float radii = collider1Radius + collider2Radius;

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
