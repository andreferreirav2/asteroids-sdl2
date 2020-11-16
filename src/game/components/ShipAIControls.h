#pragma once
#include "../../engine/ecs/Component.h"
#include <map>

enum class ShipAIStates
{
	AIM,
	SHOOT,
	REST,
	CHASE,
	FLEE
};

struct ShipAIControls : public Component
{
	// AI State Machine (Action Planner?)

	// World State?
	// Actions
	// States

	void onUpdate(float dt) { timeUntilStateChange -= dt; }
	bool canChangeState() { return timeUntilStateChange <= 0.0f; }
	bool switchState(ShipAIStates nextState)
	{
		if (!canChangeState() || state == nextState)
		{
			return false;
		}
		timeUntilStateChange = stateDurations.find(nextState)->second;
		state = nextState;

		return true;
	}

	std::map<ShipAIStates, float> const stateDurations =
	{
		{ShipAIStates::AIM, 2},
		{ShipAIStates::SHOOT, 0},
		{ShipAIStates::REST, 2},
		{ShipAIStates::FLEE, 4},
		{ShipAIStates::CHASE, 4},
	};

	ShipAIStates state = ShipAIStates::FLEE;
	float timeUntilStateChange = stateDurations.find(state)->second;
};