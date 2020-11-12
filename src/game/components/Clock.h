#pragma once
#include <SDL.h>
#include "../../engine/ecs/Component.h"

struct Clock : public Component
{
public:
	Uint32 startTicks;
	Uint32 lastFrameTicks;
	Uint32 currentTicks;

	float deltaTime = 0.0f;
	float timeScale = 1.0f;
	float fps = 0.0f;
	float mspf = 0.0f;

	// used for FPS calc
	Uint32 fpsTimeSpent;
	Uint32 fpsFrames;
};

