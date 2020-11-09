#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/Types.h"

struct RigidBody : public Component
{
	RigidBody(float mass = 1.0f, float drag = 0.0f, float vx = 0.0f, float vy = 0.0f) : mass(mass), drag(drag), velocity({ vx, vy }) {}

	float mass;
	float drag;
	float2 velocity;
};