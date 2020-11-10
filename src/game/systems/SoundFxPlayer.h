#pragma once
#include "../../engine/sdl/SDLApp.h"
#include "../../engine/ecs/ECSManager.h"

class SoundFxPlayer : public System
{
public:
	SoundFxPlayer(SDLApp& sdlApp);

	// Inherited via System
	virtual void onStart(ECSManager& manager) override;
	virtual void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt) override;

private:
	SDLApp& m_sdlApp;
};

