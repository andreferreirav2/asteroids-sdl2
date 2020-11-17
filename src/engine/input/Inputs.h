#pragma once
#include <set>
#include "../Types.h"

enum class Key
{
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,

	KEY_SPACE,

	KEY_2,
	KEY_3,

	QUIT,
	// TODO: Fill remaining
};

class Inputs
{
public:
	void update(std::set<Key> newKeys);
	void setControllerAxis(float2 leftAxis, float2 rightAxis);
	bool isPressed(Key k);
	bool isJustPressed(Key k);
	bool isJustReleased(Key k);

	std::set<Key> keys;
	std::set<Key> previousFrameKeys;
	bool controllerEnabled;
	float2 leftAxis;
	float2 rightAxis;
};
