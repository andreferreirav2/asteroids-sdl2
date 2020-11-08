#pragma once
#include "../../engine/ecs/ECSManager.h"
#include "../../engine/sdl/SDLApp.h"
#include <memory>

class SDLRenderer : public System
{
public:
	SDLRenderer(SDLApp& sdlApp);

	// Inherited via System
	void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt) override;

private:
	SDLApp& m_sdlApp;
};

