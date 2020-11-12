#pragma once
#include <SDL.h>
#include "../../engine/ecs/Component.h"

struct Clock : public Component
{
public:
	Clock(float timeScale = 1.0f, Uint32 fpsCap = 60) :
		timeScale(timeScale),
		fpsCap(fpsCap)
	{
	};

	Uint32 startTicks = 0;
	Uint32 lastFrameTicks = 0;
	Uint32 currentTicks = 0;

	float deltaTime = 0.0f;
	float timeScale = 1.0f;

	// used for FPS calc
	float fps = 0.0f;
	float mspf = 0.0f;
	Uint32 fpsCap = 60;
	Uint32 fpsTimeSpent = 0;
	Uint32 fpsFrames = 0;
};

