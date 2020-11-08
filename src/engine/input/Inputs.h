#pragma once
#include <set>

enum class Key
{
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,

	KEY_SPACE,

	QUIT,
	// TODO: Fill remaining
};

class Inputs
{
public:
	void update(std::set<Key> newKeys);
	bool isPressed(Key k);
	bool isJustPressed(Key k);
	bool isJustReleased(Key k);

private:
	std::set<Key> keys;
	std::set<Key> previousFrameKeys;
};