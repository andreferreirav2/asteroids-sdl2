#pragma once
#include "../../engine/ecs/Component.h"

struct Game : public Component
{
	Game(bool running) : running(running) {}
	bool running = false;
};