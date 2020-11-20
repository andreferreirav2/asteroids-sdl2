#pragma once
#include "../../../engine/ecs/ECSManager.h"
#include "../../../engine/sdl/SDLApp.h"
#include "../../../engine/sdl/LoadedObj.h"
#include <memory>
#include <map>

class SDL3DRenderer : public System
{
public:
	SDL3DRenderer(SDLApp& sdlApp);
	void renderHUD(ECSManager& manager);

	// Inherited via System
	virtual void onStart(ECSManager& manager) override;
	void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs) override;

private:
	SDLApp& m_sdlApp;
	std::shared_ptr<LoadedObj> m_loadedObj;
};

