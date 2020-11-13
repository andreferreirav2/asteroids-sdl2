#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/input/Inputs.h"

struct ShipManualControls : public Component
{
	ShipManualControls(Key accelerate, Key turnLeft, Key turnRight, Key shootPrimary, Key shootSecondary) :
		accelerate(accelerate),
		turnLeft(turnLeft),
		turnRight(turnRight),
		shootPrimary(shootPrimary),
		shootSecondary(shootSecondary)

	{
	}

	Key accelerate;
	Key turnLeft;
	Key turnRight;
	Key shootPrimary;
	Key shootSecondary;
};