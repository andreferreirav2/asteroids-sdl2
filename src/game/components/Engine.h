#pragma once
#include "../../engine/ecs/Component.h"

struct Engine : public Component
{
	Engine() : thrustValue(0.0f), turnValue(0.0f), thrustSpeed(1.0f), turnSpeed(1.0f) {};
	Engine(float thrustSpeed, float turnSpeed) : thrustValue(0.0f), turnValue(0.0f), thrustSpeed(thrustSpeed), turnSpeed(turnSpeed) {};

	float thrustValue;
	float turnValue;

	float thrustSpeed;
	float turnSpeed;
};