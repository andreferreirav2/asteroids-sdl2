#pragma once
#include "../../engine/ecs/ECSManager.h"
#include "../../engine/sdl/SDLApp.h"
#include "../components/SpriteSDL.h"
#include <memory>
#include <map>

class SDLRenderer : public System
{
public:
	SDLRenderer(SDLApp& sdlApp);
	void setTexture(std::shared_ptr<SpriteSDL> sprite);

	// Inherited via System
	virtual void onStart(ECSManager& manager) override;
	void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs) override;

private:
	void drawSpriteSDL(std::shared_ptr<SpriteSDL>& sprite, float2 position, float rotation = 0.0f, float2 scale = float2({ 1.0f, 1.0f }));
	std::map<int, std::shared_ptr<LoadedTexture>> m_scoresTextTextureCache;
	SDLApp& m_sdlApp;
};

