#include "TimePassing.h"
#include "../components/Clock.h"
#include <SDL.h>
#include <iostream>

#define CALC_FPS_EVERY_MS 1000.0f

void TimePassing::onStart(ECSManager& manager)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<Clock>())
	{
		auto clock = manager.getComponentOfType<Clock>(e);
		if (clock)
		{
			Uint32 currentTicks = SDL_GetTicks();
			clock->startTicks = currentTicks;
			clock->lastFrameTicks = currentTicks;
			clock->currentTicks = currentTicks;
			clock->fpsFrames = 0;
		}
	}
}

void TimePassing::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	// What if there are no clocks? or more than one?
	auto clock = manager.getAllComponentsOfType<Clock>()[0];

	// Update last frame's info
	clock->lastFrameTicks = clock->currentTicks;

	// Force FPS cap
	Uint32 ticksLeftForCap = SDL_GetTicks() - clock->lastFrameTicks;
	Uint32 screenTicksPerFrame = 1000 / clock->fpsCap;
	if (ticksLeftForCap < screenTicksPerFrame)
	{
		SDL_Delay(screenTicksPerFrame - ticksLeftForCap);
	}

	Uint32 currentTicks = SDL_GetTicks();
	clock->deltaTime = static_cast<float>(currentTicks - clock->lastFrameTicks) * clock->timeScale / 1000.0f;
	clock->currentTicks = currentTicks;

	clock->fpsFrames++;
	if (currentTicks - clock->fpsTimeSpent >= CALC_FPS_EVERY_MS)
	{
		clock->fps = clock->fpsFrames * 1000 / CALC_FPS_EVERY_MS;
		clock->mspf = CALC_FPS_EVERY_MS / clock->fps;

		clock->fpsFrames = 0;
		clock->fpsTimeSpent = currentTicks;

		std::cerr << clock->fps << std::endl;
	}
}
