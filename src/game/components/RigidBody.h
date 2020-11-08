#pragma once
#include "../../engine/ecs/Component.h"
#include "../utils/Utils.h"

struct RigidBody : public Component
{
	RigidBody() : mass(1.0f), velocity({ 0.0f, 0.0f }) {}
	RigidBody(float mass, float vx, float vy) : mass(mass), velocity({ vx, vy }) {}

	float mass;
	vec2 velocity;
};