#pragma once
#include "../../engine/ecs/Component.h"

struct Engine : public Component
{
	Engine(float thrustSpeed = 0.0f, float turnSpeed = 0.0f) : thrustValue(0.0f), turnValue(0.0f), thrustSpeed(thrustSpeed), turnSpeed(turnSpeed) {};

	float thrustValue;
	float turnValue;

	const float thrustSpeed;
	const float turnSpeed;
};