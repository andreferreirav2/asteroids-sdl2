#include "TimePassing.h"
#include "../components/Clock.h"
#include <SDL.h>

void TimePassing::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<Clock>())
	{
		auto clock = manager.getComponentOfType<Clock>(e);
		if (clock)
		{
			Uint32 currentTicks = SDL_GetTicks();
			clock->deltaTime = static_cast<float>(currentTicks - clock->lastFrameTicks) * clock->timeScale / 1000.0f;
			clock->lastFrameTicks = currentTicks;
		}
	}
}
