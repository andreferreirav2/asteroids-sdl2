#include "PhysicsCollisions.h"
#include "../components/CircleCollider.h"
#include "../components/Transform.h"
#include "iostream"
#include <vector>
#include <tuple>


void PhysicsCollisions::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	std::set<Entity> entitiesSet = manager.getAllEntitiesWithComponentType<CircleCollider, Transform>();
	size_t entitiesSize = entitiesSet.size();

	// Pre-fetch all components
	auto entities = std::vector<Entity>(entitiesSize);
	auto colliders = std::vector<std::shared_ptr<CircleCollider>>(entitiesSize);
	auto transforms = std::vector<std::shared_ptr<Transform>>(entitiesSize);
	int i = 0;
	for (auto ent : entitiesSet)
	{
		entities[i] = ent;
		colliders[i] = manager.getComponentOfType<CircleCollider>(ent);
		transforms[i] = manager.getComponentOfType<Transform>(ent);
		++i;
	}

	// TODO: check groups of layers against other groups of colliding layers, instead of all to all
	// O(n2)
	for (int i = 0; i < entitiesSize; ++i)
	{
		Entity e1 = entities[i];
		auto const& collider1 = colliders[i];
		auto const& transform1 = transforms[i];
		auto const& transform1Position = transform1->position;
		auto const& collider1Radius = collider1->radius;
		auto const& collider1Layer = collider1->layer;
		auto const& collider1CollidesWith = collider1->collidesWith;

		for (int j = i + 1; j < entitiesSize; ++j)
		{
			Entity e2 = entities[j];
			auto const& collider2 = colliders[j];
			auto const& transform2 = transforms[j];
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
				}
			}
		}
	}
}
