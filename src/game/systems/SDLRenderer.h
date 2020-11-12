#pragma once
#include "../../engine/ecs/ECSManager.h"
#include "../../engine/sdl/SDLApp.h"
#include <memory>

class SDLRenderer : public System
{
public:
	SDLRenderer(SDLApp& sdlApp);

	// Inherited via System
	virtual void onStart(ECSManager& manager) override;
	void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs) override;

private:
	SDLApp& m_sdlApp;
};

