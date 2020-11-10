#include "Inputs.h"
#include <set>

void Inputs::update(std::set<Key> newKeys)
{
	previousFrameKeys = keys;
	keys = newKeys;
}

void Inputs::setControllerAxis(float2 lAxis, float2 rAxis)
{
	leftAxis.x = lAxis.x;
	leftAxis.y = lAxis.y;
	rightAxis.x = rAxis.x;
	rightAxis.y = rAxis.y;
	controllerEnabled = true;
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