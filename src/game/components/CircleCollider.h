#pragma once
#include "../../engine/ecs/Entity.h"
#include "../../engine/ecs/Component.h"
#include <set>

struct CircleCollider : public Component
{
	CircleCollider(float radius = 0.0f, int layer = 0, int collidesWith = 0) :
		radius(radius),
		layer(layer),
		collidesWith(collidesWith),
		collidingEntities({})
	{
	}

	float radius = 0.0f;
	int layer = 0;
	int collidesWith = 0;
	std::set<Entity> collidingEntities = {};
};