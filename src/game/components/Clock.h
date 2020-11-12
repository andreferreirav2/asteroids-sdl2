#pragma once
#include <SDL.h>
#include "../../engine/ecs/Component.h"

struct Clock : public Component
{
public:
	Uint32 lastFrameTicks;

	float deltaTime = 0.0f;
	float timeScale = 1.0f;
	bool paused = false;
};

