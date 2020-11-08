#include "Inputs.h"
#include <set>

void Inputs::update(std::set<Key> newKeys)
{
	previousFrameKeys = keys;
	keys = newKeys;
}

bool Inputs::isPressed(Key k)
{
	return keys.find(k) != keys.end();
}

bool Inputs::isJustPressed(Key k)
{
	return isPressed(k) && previousFrameKeys.find(k) == previousFrameKeys.end();
}

bool Inputs::isJustReleased(Key k)
{
	return !isPressed(k) && previousFrameKeys.find(k) != previousFrameKeys.end();
}