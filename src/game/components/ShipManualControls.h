#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/input/Inputs.h"

struct ShipManualControls : public Component
{
	ShipManualControls(Key accelerate, Key turnLeft, Key turnRight, Key shoot) :
		accelerate(accelerate),
		turnLeft(turnLeft),
		turnRight(turnRight),
		shoot(shoot)

	{
	}

	Key accelerate;
	Key turnLeft;
	Key turnRight;
	Key shoot;
};